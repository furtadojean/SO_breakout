#include "ball.h"
#include <cmath> // For std::sqrt and std::pow
#include <iostream>

// Constructor for Ball, inherits from CollisionObject
Ball::Ball(std::array<float, 2> center, int hwidth, int hheight, std::array<float, 2> direction)
    : CollisionObject(center, hwidth, hheight, 'O') {
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
    cout << "Ball center: " << getCenter()[0] << ", " << getCenter()[1] << endl;
    cout << "Ball direction: " << direction[0] << ", " << direction[1] << endl;
}

// Override onCollision to update the direction vector based on the collision
void Ball::onCollision(Object& object) {
    // Get the difference between the centers
    std::array<float, 2> otherCenter = object.getCenter();
    float dx = otherCenter[1] - getCenter()[1];
    float dy = otherCenter[0] - getCenter()[0];

    // Normalize the difference vector (direction towards the other object)
    double norm = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    if (norm != 0) {
        dx /= norm;
        dy /= norm;
    }

    // Update the direction vector: add the difference to the current direction
    direction[0] = dx;
    direction[1] = dy;

    // Normalize the direction vector to ensure its norm is always 1
    normalizeDirection();

    // Call the base class's onCollision function (optional)
    CollisionObject::onCollision(object);
}
