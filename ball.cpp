#include "ball.h"
#include <cmath> // For std::sqrt and std::pow
#include <iostream>

// Constructor for Ball, inherits from CollisionObject
Ball::Ball(std::array<float, 2> center, int hwidth, int hheight, std::array<float, 2> direction, CollisionManager* manager)
    : CollisionObject(center, hwidth, hheight, 'O', manager) {
    this->direction = direction;
    normalizeDirection(); // Ensure the direction vector is normalized
}

// Normalize the direction vector so its norm is always 1
void Ball::normalizeDirection() {
    double norm = std::sqrt(std::pow(direction[0], 2) + std::pow(direction[1], 2));
    if (norm != 0) {
        direction[0] /= norm;
        direction[1] /= norm;
    }
}

// Move the ball by adding the direction vector to the center
void Ball::onClockTick() {
    std::array<float, 2> center = getCenter();
    center[0] += direction[0];
    center[1] += direction[1];
    setCenter(center);
    //cout << "Ball center: " << getCenter()[0] << ", " << getCenter()[1] << endl;
    //cout << "Ball direction: " << direction[0] << ", " << direction[1] << endl;
}

// Reflect the direction vector across a given normal
void Ball::reflect(const std::array<float, 2>& normal) {
    float dotProduct = direction[0] * normal[0] + direction[1] * normal[1];
    direction[0] -= 2 * dotProduct * normal[0];
    direction[1] -= 2 * dotProduct * normal[1];
}

// Override onCollision to update the direction vector based on the collision
void Ball::onCollision(Object& object) {
    // Handle the collision logic here
    getCollisionManager()->acquireSemaphore();  // Protect critical section with semaphore

    //std::cout << "Collision detected!" << std::endl;
    // Get the difference between the centers
    //std::array<float, 2> otherCenter = object.getCenter();
    //float dx = -otherCenter[0] + getCenter()[0];
    //float dy = -otherCenter[1] + getCenter()[1];

    //// Normalize the difference vector (direction towards the other object)
    //double norm = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    //if (norm != 0) {
    //    dx /= norm/0.6;
    //    dy /= norm/0.8;
    //}

    //// Update the direction vector: add the difference to the current direction
    //float invert = -1.0 + (rand() % 2) * 2;
    //direction[0] = dx * invert;
    //direction[1] = dy;

    // Get the object's center and half-dimensions
    auto [objectCenterX, objectCenterY] = object.getCenter();
    float objectHalfWidth = object.getHWidth();
    float objectHalfHeight = object.getHHeight();
    auto [centerX, centerY] = getCenter();
    float halfWidth = getHWidth();
    float halfHeight = getHHeight();

    bool hitX = centerX + halfWidth > objectCenterX - objectHalfWidth && centerX - halfWidth < objectCenterX + objectHalfWidth;
    bool hitY = centerY + halfHeight > objectCenterY - objectHalfHeight && centerY - halfHeight < objectCenterY + objectHalfHeight;

    if (hitX && hitY) {
        // Reflect off the right or left (vertical collision)
        if (centerX + halfWidth > objectCenterX + objectHalfWidth || centerX - halfWidth < objectCenterX - objectHalfWidth) {
            std::array<float, 2> normal = {centerX > objectCenterX ? 1.0f : -1.0f, 0.0f};
            reflect(normal);
        }
        // Reflect off the top or bottom (horizontal collision)
        if (centerY + halfHeight > objectCenterY + objectHalfHeight || centerY - halfHeight < objectCenterY - objectHalfHeight) {
            std::array<float, 2> normal = {0.0f, centerY > objectCenterY ? 1.0f : -1.0f};
            reflect(normal);
        }
    }

    // Normalize the direction vector to ensure its norm is always 1
    normalizeDirection();

    // Wait for collision to end
    getCollisionManager()->waitForEndCollision(&object);

    // End collision, allowing other threads to proceed
    getCollisionManager()->releaseSemaphore();

    // Call the base class's onCollision function (optional)
    //CollisionObject::onCollision(object);
}
