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

    virtual void onClockTick() {};

    bool operator ==(const Object& other) const {
        return this == &other;
    }
};

#include <vector>
#include <map>
#include <array>
#include <string>
#include "manager.h"

class CollisionObject : public Object {
private:
    std::vector<Object*> collisionObjects;
    bool collisionActive;
    CollisionManager* collisionManager;  // CollisionManager instance

public:
    // Constructor
    CollisionObject(std::array<float, 2> center, int hwidth, int hheight, char symbol, CollisionManager* manager);
    ~CollisionObject();

    // Methods for managing collision objects
    void addCollisionObject(Object& object);
    void removeCollisionObject(Object& object);
    std::vector<Object*> getCollisionObjects();

    // Collision detection
    virtual void onCollision(Object& object);
    void checkCollision();
    void onClockTick() override {};

    // Collision management
    bool isCollisionActive();
    void setCollisionActive(bool active);

    CollisionManager* getCollisionManager();
};
