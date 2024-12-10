#pragma once

#include "object.h"
#include <map>

class Draw {
private:
    int rows, cols;  // Screen size in characters (rows x cols)
    std::vector<Object*> objects;  // List of objects to draw
    std::map<std::pair<int, int>, Object*> pixelMap;  // Map to track which object occupies each pixel
    int score = 0;

public:
    // Constructor that initializes the screen size
    Draw(int rows, int cols);

    // Method to update the list of objects to be drawn
    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    vector<Object*> setObjects(std::vector<T*>& objects) {
        // Clear this->objects and add the new objects
        this->objects.clear();
        for (auto& obj : objects) {
            this->objects.push_back(obj);
        }
        return this->objects;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    vector<Object*> addObjects(std::vector<T*>& objects) {
        // Add the new objects to this->objects
        for (auto& obj : objects) {
            this->objects.push_back(obj);
        }
        return this->objects;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    Object addObject(T& object) {
        // Add the new object to this->objects
        this->objects.push_back(&object);
        return object;
    }


    // Function to update the pixelMap based on the objects' positions
    void update();

    // Function to draw the screen based on the pixelMap
    void draw();

    // Helper function to clear the screen (for clean drawing)
    void clear();

    // Function to update the score
    void increaseScore();
};
