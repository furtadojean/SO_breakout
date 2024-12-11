#include "draw.h"
#include <iostream>

// Constructor for the Draw class
Draw::Draw(int rows, int cols) : rows(rows), cols(cols) {}


// Helper function to clear the screen by printing newlines
void Draw::clear() {
    // ANSI escape code to clear screen and reset cursor position
    std::cout << "\033[2J\033[H";
}

// Function to update the pixelMap based on the objects' positions
void Draw::update() {
    // Reset the pixel map before updating
    pixelMap.clear();

    // Update the map based on the objects' centers and dimensions
    for (auto& obj : objects) {
        //obj->onClockTick();
        if (obj->getDraw() == false) {
            continue;
        }
        int centerX = obj->getCenter()[0];
        int centerY = obj->getCenter()[1];
        int halfWidth = obj->getHWidth();
        int halfHeight = obj->getHHeight();

        // Loop through the area occupied by the object (in terms of screen pixels)
        for (int y = centerY - halfHeight; y <= centerY + halfHeight; ++y) {
            for (int x = centerX - halfWidth; x <= centerX + halfWidth; ++x) {
                // Check if the pixel is within screen bounds
                if (x >= 0 && x < cols && y >= 0 && y < rows) {
                    // Map the (x, y) position to the object
                    pixelMap[{x, y}] = obj;
                }
            }
        }
    }
}

// Function to draw the screen based on the pixelMap
void Draw::draw() {
    // Clear the previous screen
    clear();

    std::cout << std::string((cols+2)/2-3, ' ') << "Score: " << score << std::endl;
    std::cout << std::string(cols+2, '-') << std::endl;
    // Now draw the screen by iterating through each pixel
    for (int y = 0; y < rows; ++y) {
        std::cout << '|';
        for (int x = 0; x < cols; ++x) {
            // Check if a position is occupied by an object
            auto it = pixelMap.find({x, y});
            if (it != pixelMap.end()) {
                // If occupied, print the object's character
                if (it->second == nullptr) {
                    std::cout << ' ';
                    continue;
                }
                std::cout << it->second->getSymbol();
            } else {
                // If not occupied, print a whitespace
                std::cout << ' ';
            }
        }
        std::cout << '|' << std::endl;
    }
    std::cout << std::string(cols+2, '-') << std::endl;
}

void Draw::increaseScore() {
    this->score++;
}

int Draw::getScore() {
    return this->score;
}
