#pragma once

#include "object.h"

class Ball : public CollisionObject {
private:
    std::array<float, 2> direction; // 2D direction vector

    // Helper function to normalize the direction vector
    void normalizeDirection();

public:
    // Constructor
    Ball(std::array<float, 2> center, int hwidth, int hheight, std::array<float, 2> direction);

    // Move the ball by adding the direction vector to its center
    void onClockTick() override;

    // Override the onCollision function to update the direction vector
    void onCollision(Object& object) override;

    Ball(const Ball&) = delete; // Delete copy constructor
    Ball& operator=(const Ball&) = delete; // Delete copy assignment
    Ball(Ball&&) = delete; // Delete move constructor
    Ball& operator=(Ball&&) = delete; // Delete move assignment
};
