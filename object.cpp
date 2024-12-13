#include "object.h"
#include "running.h"
#include <algorithm>
#include <thread>

// Object class constructor
Object::Object(array<float, 2> center, int hwidth, int hheight, char symbol) 
    : center(center), hwidth(hwidth), hheight(hheight), symbol(symbol) {}

// Getter methods
array<float, 2> Object::getCenter() {
    return center;
}

int Object::getHWidth() {
    return hwidth;
}

int Object::getHHeight() {
    return hheight;
}

char Object::getSymbol() {
    return symbol;
}

bool Object::getDraw() {
    return draw;
}

// Setter methods
void Object::setCenter(array<float, 2> center) {
    this->center = center;
}

void Object::setHWidth(int hwidth) {
    this->hwidth = hwidth;
}

void Object::setHHeight(int hheight) {
    this->hheight = hheight;
}

void Object::setSymbol(char symbol) {
    this->symbol = symbol;
}

void Object::setDraw(bool draw) {
    this->draw = draw;
}

CollisionObject::CollisionObject(std::array<float, 2> center, int hwidth, int hheight, char symbol, CollisionManager* manager)
    : Object(center, hwidth, hheight, symbol), collisionActive(true), collisionManager(manager) {
}

CollisionObject::~CollisionObject() {}

void CollisionObject::addCollisionObject(Object& object) {
    collisionObjects.push_back(&object);
}

void CollisionObject::removeCollisionObject(Object& object) {
    collisionObjects.erase(std::find(collisionObjects.begin(), collisionObjects.end(), &object));
}

std::vector<Object*> CollisionObject::getCollisionObjects() {
    return collisionObjects;
}

void CollisionObject::onCollision(Object& object) {
    // Handle the collision logic here
    collisionManager->acquireSemaphore();  // Protect critical section with semaphore

    // Wait for collision to end
    collisionManager->waitForEndCollision(&object);

    // End collision, allowing other threads to proceed
    collisionManager->releaseSemaphore();
}

void CollisionObject::checkCollision() {
    if (!isCollisionActive()) {
        return;
    }

    while (true) {
        if (Running::getInstance().shouldStop()) {
            break;
        }
        for (auto& object : collisionObjects) {
            if (!object->getHasPhysics()) {
                continue;
            }

            bool isColliding = std::abs(getCenter()[0] - object->getCenter()[0]) < getHWidth() + object->getHWidth() &&
                                std::abs(getCenter()[1] - object->getCenter()[1]) < getHHeight() + object->getHHeight();

            bool wasColliding = collisionManager->collisionStatus[object];

            if (collisionActive && isColliding && !wasColliding) {
                collisionManager->collisionStatus[object] = true;
                std::thread([this, &object]() {
                    onCollision(dynamic_cast<Object&>(*object));
                }).detach();
            }
            if (!isColliding && wasColliding) {
                collisionManager->endCollision(object);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool CollisionObject::isCollisionActive() {
    return collisionActive;
}

void CollisionObject::setCollisionActive(bool active) {
    collisionActive = active;
}

CollisionManager* CollisionObject::getCollisionManager() {
    return collisionManager;
}

// Implement the getter and setter for hasPhysics
bool Object::getHasPhysics() {
    return hasPhysics;
}
void Object::setHasPhysics(bool hasPhysics) {
    this->hasPhysics = hasPhysics;
}
