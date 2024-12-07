#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <map>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>
#include <thread>
using namespace std;

// TODO: an object should have an offset function so it can
// keep the same global position, but have a local position that will be
// used when checking collision and drawing on screen

class Object {
private:
    array<float, 2> center;
    int hwidth, hheight;
    char symbol;
    bool hasPhysics = true;
    bool draw = true;

public:
    // Constructor
    Object(array<float, 2> center, int hwidth, int hheight, char symbol);

    // Getter and Setter methods
    array<float, 2> getCenter();
    int getHWidth();
    int getHHeight();
    char getSymbol();
    bool getHasPhysics();
    bool getDraw();
    void setCenter(array<float, 2> center);
    void setHWidth(int hwidth);
    void setHHeight(int hheight);
    void setSymbol(char symbol);
    void setHasPhysics(bool hasPhysics);
    void setDraw(bool draw);

    virtual void onClockTick() {
        cout << "Base class" << endl;
    };

    bool operator ==(const Object& other) const {
        return this == &other;
    }
};

class CollisionObject : public Object {
private:
    vector<Object*> collisionObjects;
    bool collisionActive;
    std::map<Object*, bool> collisionStatus;
    std::mutex mtx;                  // Mutex to protect the critical section
    std::condition_variable cv;      // Condition variable to wait for collision to end
    sem_t semaphore;
    std::map<Object*, std::condition_variable> collisionCVs;  // CVs for each object

public:
    // Constructor
    CollisionObject(array<float, 2> center, int hwidth, int hheight, char symbol);
    ~CollisionObject();

    CollisionObject() = default;
    // Delete copy and move constructors/assignments for CollisionObject
    CollisionObject(const CollisionObject&) = delete;
    CollisionObject& operator=(const CollisionObject&) = delete;
    CollisionObject(CollisionObject&&) = delete;
    CollisionObject& operator=(CollisionObject&&) = delete;

    // Methods for managing collision objects
    void addCollisionObject(Object& object);
    void removeCollisionObject(Object& object);
    vector<Object*> getCollisionObjects();
    
    // Collision detection
    virtual void onCollision(Object& object);
    void checkCollision();

    void onClockTick() override {};

    // Collision management
    bool isCollisionActive();
    void setCollisionActive(bool active);

    void endCollision(Object* object);
    void waitForEndCollision(Object* object);
};
