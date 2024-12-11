#pragma once

#include <mutex>
#include <condition_variable>
#include <semaphore.h>
#include <map>
#include <stdexcept>

class Object; // Forward declaration of Object class

class CollisionManager {
private:
    std::mutex mtx;                                // Mutex to protect the critical section
    sem_t semaphore;                               // Semaphore to control access
    std::map<Object*, std::condition_variable> collisionCVs;  // Condition variables per object

public:
    std::map<Object*, bool> collisionStatus;       // Track collision status for objects

    // Constructor
    CollisionManager();

    // Destructor
    ~CollisionManager();

    // Semaphore operations
    void acquireSemaphore();
    void releaseSemaphore();

    // Methods for managing collisions
    void waitForEndCollision(Object* object);
    void endCollision(Object* object);
};
