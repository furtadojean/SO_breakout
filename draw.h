#pragma once

#include "object.h"
#include <map>

class Draw {
private:
    int rows, cols;  // Screen size in characters (rows x cols)
    std::vector<std::shared_ptr<Object>> objects;  // List of objects to draw
    std::map<std::pair<int, int>, std::shared_ptr<Object>> pixelMap;  // Map to track which object occupies each pixel

public:
    // Constructor that initializes the screen size
    Draw(int rows, int cols);

    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    void setObjects(std::vector<std::shared_ptr<T>>& objects) {
        // Clear this->objects and add the new objects
        this->objects.clear();
        for (auto& obj : objects) {
            this->objects.push_back(obj);  // Shared ownership, no need to move
        }
        //return this->objects;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    void addObjects(std::vector<std::shared_ptr<T>>& objects) {
        // Add the new objects to this->objects
        for (auto& obj : objects) {
            this->objects.push_back(obj);  // Shared ownership, no need to move
        }
        //return this->objects;
    }


    //// Modified function with r-value reference to move the object
    //template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    //std::shared_ptr<Object>& addObject(T&& object) {
    //    // Move the object into a shared_ptr
    //    auto sharedObject = std::make_shared<T>(std::forward<T>(object));
    //    this->objects.push_back(sharedObject);
    //    return this->objects.back();
    //}

    // Modified function with r-value reference to move the object
    template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>, bool> = true>
    void addObject(std::shared_ptr<T>& object) {
        // Move the object into a shared_ptr
        this->objects.push_back(std::move(object));
        //return object;
    }


    // Function to update the pixelMap based on the objects' positions
    void update();

    // Function to draw the screen based on the pixelMap
    void draw();

    // Helper function to clear the screen (for clean drawing)
    void clear();
};
