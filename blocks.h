#pragma once
#include "object.h"  // Include the Object and CollisionObject classes
#include "ball.h"
#include "manager.h"

class BlockObject : public CollisionObject {
public:
    // Constructor for BlockObject, initializing as a CollisionObject
    BlockObject(array<float, 2> center, int hwidth, int hheight, char symbol, CollisionManager* manager)
        : CollisionObject(center, hwidth, hheight, symbol, manager) {setCollisionActive(true);}

    // Override the onCollision method to disable collision when a collision occurs
    void onCollision(Object& object) override {
        setCollisionActive(false);
        setDraw(false);
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

public:
    // Constructor for Blocks class
    Blocks(int totalWidth, int totalHeight, int blocksPerRow, int numberOfRows, Ball& ball, CollisionManager* manager);

    // Method to get the list of blocks
    std::vector<BlockObject*> getBlocks();

    // Method to populate blocks based on total size and number of blocks
    void populateBlocks();
};
