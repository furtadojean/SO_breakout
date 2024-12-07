#pragma once

#include "object.h"
#include <array>

class Bar : public Object {
public:
    // Constructor to initialize the Bar object
    Bar(int totalWidth, int totalHeight, char symbol);

    // Method to move the bar horizontally by an x amount
    void moveHorizontally(int x);
    void onClockTick() override {};

private:
    // Bar width and height are calculated based on totalWidth and totalHeight
    void calculateDimensions(int totalWidth, int totalHeight);
};
