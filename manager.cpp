#include "manager.h"
#include <semaphore.h>
#include <iostream>
#include <chrono>
#include <thread>

CollisionManager::CollisionManager() {
    if (sem_init(&semaphore, 0, 1) != 0) {
        throw std::runtime_error("Failed to initialize semaphore");
    }
}

CollisionManager::~CollisionManager() {
    sem_destroy(&semaphore);
}

void CollisionManager::acquireSemaphore() {
    sem_wait(&semaphore);  // Acquire semaphore before entering critical section
}

void CollisionManager::releaseSemaphore() {
    sem_post(&semaphore);  // Release the semaphore
}

void CollisionManager::waitForEndCollision(Object* object) {
    std::unique_lock<std::mutex> lock(mtx);
    collisionCVs[object].wait(lock, [this, object]() { 
        return !collisionStatus[object];  // Wait until the collision ends
    });
}

void CollisionManager::endCollision(Object* object) {
    std::lock_guard<std::mutex> lock(mtx);
    collisionStatus[object] = false;  // Mark the object as no longer colliding
    collisionCVs[object].notify_one();  // Notify the waiting thread
}
