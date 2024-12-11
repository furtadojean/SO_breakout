#include "running.h"

#include <atomic>

// Set the flag to stop threads
void Running::stopThreads() {
    stopFlag.store(true);
}

// Check if threads should stop
bool Running::shouldStop() const {
    return stopFlag.load();
}

// Constructor is private to enforce singleton
Running::Running() : stopFlag(false) {}
