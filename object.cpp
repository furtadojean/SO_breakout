#include "object.h"
#include <iostream>
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
// CollisionObject class constructor
CollisionObject::CollisionObject(array<float, 2> center, int hwidth, int hheight, char symbol)
    : Object(center, hwidth, hheight, symbol) {
        setCollisionActive(true);
        if (sem_init(&semaphore, 0, 1) != 0) {
            throw std::runtime_error("Failed to initialize semaphore");
        }
    }

CollisionObject::~CollisionObject() {
    sem_destroy(&semaphore);
}

// Add a collision object to the list
void CollisionObject::addCollisionObject(Object& object) {
    collisionObjects.push_back(&object);
}

// Remove a collision object from the list
void CollisionObject::removeCollisionObject(Object& object) {
    collisionObjects.erase(find(collisionObjects.begin(), collisionObjects.end(), &object));
}

// Getter for the collision objects
vector<Object*> CollisionObject::getCollisionObjects() {
    return collisionObjects;
}

// Method to handle the collision detection
void CollisionObject::onCollision(Object& object) {
    std::unique_lock<std::mutex> lock(mtx);  // Lock the mutex for critical section

    // Ensure that the thread can only execute the collision handling if it is not colliding
    sem_wait(&semaphore);  // Acquire semaphore before entering critical section

    cout << "Collision detected!" << endl;

    // Wait until the collision ends
    waitForEndCollision(&object);

    // Once the collision ends, release the semaphore and allow other threads to proceed
    sem_post(&semaphore);
}

// Check if any collision occurs with the stored objects
void CollisionObject::checkCollision() {
    if (!isCollisionActive()) {
        return;
    }
    while (true) {
        for (auto& object : collisionObjects) {
            if (!object->getHasPhysics()) {
                continue;
            }
            // Check for collision based on the bounding box overlap
            bool isColliding = abs(getCenter()[0] - object->getCenter()[0]) < getHWidth() + object->getHWidth() &&
                                abs(getCenter()[1] - object->getCenter()[1]) < getHHeight() + object->getHHeight();

            // Retrieve current collision status for this object
            bool wasColliding = collisionStatus[object];

            // If the object is colliding and wasn't colliding previously, start a new thread for the collision
            if (isColliding && !wasColliding) {
                collisionStatus[object] = true;  // Update collision status to true
                std::thread([this, &object]() { 
                    onCollision(dynamic_cast<Object&>(*object)); 
                }).detach();
            }
            // If the object was colliding but is no longer colliding, notify the end of collision
            else if (!isColliding && wasColliding) {
                collisionStatus[object] = false;  // Update collision status to false
                // Call endCollision() to notify the thread that the collision has ended
                this->endCollision(object);
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void CollisionObject::endCollision(Object* object) {
     std::lock_guard<std::mutex> lock(mtx);
    
    // Notify the thread that is waiting for the collision to end
    auto& cv = collisionCVs[object];
    cv.notify_one();
}

void CollisionObject::waitForEndCollision(Object* object) {
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until the collision ends
    collisionCVs[object].wait(lock, [this, object]() { 
        return !collisionStatus[object];  // Wait until collision is not active
    });
}

// Implement the getter and setter for collisionActive
bool CollisionObject::isCollisionActive() {
    return collisionActive;
}

void CollisionObject::setCollisionActive(bool active) {
    collisionActive = active;
}

// Implement the getter and setter for hasPhysics
bool Object::getHasPhysics() {
    return hasPhysics;
}
void Object::setHasPhysics(bool hasPhysics) {
    this->hasPhysics = hasPhysics;
}
