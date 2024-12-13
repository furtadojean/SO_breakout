#pragma once

#include "object.h"

class Ball : public CollisionObject {
private:
    std::array<float, 2> direction; // 2D direction vector
    std::array<int, 2> max; // Bounds

    // Helper function to normalize the direction vector
    void normalizeDirection();

public:
    // Constructor
    Ball(std::array<float, 2> center, int hwidth, int hheight, std::array<float, 2> direction, CollisionManager* manager, std::array<int, 2> max);

    // Move the ball by adding the direction vector to its center
    void onClockTick() override;

    // Override the onCollision function to update the direction vector
    void onCollision(Object& object) override;

    void reflect(const std::array<float, 2>& normal);

    // Getter method for the direction vector
    std::array<float, 2> getDirection() {
        return direction;
    }

    void fixEdgeCase();
};
