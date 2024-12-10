#pragma once
#include "object.h"  // Include the Object and CollisionObject classes
#include "ball.h"
#include "manager.h"
#include "draw.h"

class BlockObject : public CollisionObject {
private:
    Draw* draw;
public:
    // Constructor for BlockObject, initializing as a CollisionObject
    BlockObject(array<float, 2> center, int hwidth, int hheight, char symbol, CollisionManager* manager, Draw* draw)
        : CollisionObject(center, hwidth, hheight, symbol, manager), draw(draw) {setCollisionActive(true);}

    // Override the onCollision method to disable collision when a collision occurs
    void onCollision(Object& object) override {
        // Handle the collision logic here
        getCollisionManager()->acquireSemaphore();  // Protect critical section with semaphore
        if(!getDraw()) {
            getCollisionManager()->releaseSemaphore();
            return;
        }
        setCollisionActive(false);
        setDraw(false);
        draw->increaseScore();
        std::cout << "Collision detected!" << std::endl;

        // Wait for collision to end
        getCollisionManager()->waitForEndCollision(&object);

        // End collision, allowing other threads to proceed
        getCollisionManager()->releaseSemaphore();
    }
    void onClockTick() override {};
};

class Blocks {
private:
    std::vector<BlockObject> blocks;  // List of BlockObject instances
    int totalWidth;  // Total width of the block section
    int totalHeight;  // Total width of the block section
    int blocksPerRow;   // Number of blocks per row
    int numberOfRows; // Number of rows of blocks
    Ball& ball;
    CollisionManager* manager;
    Draw* draw;
    std::vector<std::thread> threads;

public:
    // Constructor for Blocks class
    Blocks(int totalWidth, int totalHeight, int blocksPerRow, int numberOfRows, Ball& ball, CollisionManager* manager, Draw* draw);

    // Method to get the list of blocks
    std::vector<BlockObject*> getBlocks();

    // Method to populate blocks based on total size and number of blocks
    void populateBlocks();

    std::vector<std::thread> getThreads() {
        return std::move(threads);
    }
};
