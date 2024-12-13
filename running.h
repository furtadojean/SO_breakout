#pragma once

#include <atomic>

class Running {
public:
    // Get the instance of the singleton
    static Running& getInstance() {
        static Running instance;
        return instance;
    }

    // Set the flag to stop threads
    void stopThreads();

    // Check if threads should stop
    bool shouldStop() const;

private:
    // Constructor is private to enforce singleton
    Running();

    // Delete copy constructor and assignment operator to avoid multiple instances
    Running(const Running&) = delete;
    Running& operator=(const Running&) = delete;

    std::atomic<bool> stopFlag; // Flag to control thread termination
};
