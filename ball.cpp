#include "ball.h"
#include <cmath> // For std::sqrt and std::pow

// Constructor for Ball, inherits from CollisionObject
Ball::Ball(std::array<float, 2> center, int hwidth, int hheight, std::array<float, 2> direction, CollisionManager* manager, std::array<int, 2> max)
    : CollisionObject(center, hwidth, hheight, 'O', manager), max(max) {
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
}
