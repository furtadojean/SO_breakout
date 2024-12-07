#include "bar.h"

// Constructor to initialize the Bar object with total width, total height, and symbol
Bar::Bar(int totalWidth, int totalHeight, char symbol)
    : Object({0, 0}, 0, 0, symbol) {  // Initialize base class with default values for now
    calculateDimensions(totalWidth, totalHeight);  // Calculate and set the width, height, and center
}

// Method to calculate the width, height, and center of the Bar based on totalWidth and totalHeight
void Bar::calculateDimensions(int totalWidth, int totalHeight) {
    int width = totalWidth / 5;  // Width is a fifth of the total width
    int height = 0;      // Height is a tenth of the width

    // Set width and height for the bar
    setHWidth(width);
    setHHeight(height);

    // Calculate center: X is half of totalWidth, Y is at the bottom fifth of totalHeight
    array<float, 2> center = {static_cast<float>(totalWidth / 2.0), static_cast<float>(totalHeight * 4 / 5.0)};  // Bottom fifth of the height
    setCenter(center);  // Set the calculated center of the bar
}

// Method to move the bar horizontally by an x amount
void Bar::moveHorizontally(int x) {
    array<float, 2> currentCenter = getCenter();
    currentCenter[0] += x;  // Move the center horizontally by x amount
    setCenter(currentCenter);  // Update the center of the Bar
}
