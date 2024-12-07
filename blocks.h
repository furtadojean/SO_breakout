#pragma once
#include "object.h"  // Include the Object and CollisionObject classes
#include "ball.h"

class BlockObject : public CollisionObject {
public:
    // Constructor for BlockObject, initializing as a CollisionObject
    BlockObject(array<float, 2> center, int hwidth, int hheight, char symbol)
        : CollisionObject(center, hwidth, hheight, symbol) {setCollisionActive(true);}

    // Override the onCollision method to disable collision when a collision occurs
    void onCollision(Object& object) override {
        setCollisionActive(false);
        setDraw(false);
    }
    void onClockTick() override {};
};

class Blocks {
private:
    std::vector<std::shared_ptr<BlockObject>> blocks;  // List of BlockObject instances
    int totalWidth;  // Total width of the block section
    int numBlocks;   // Number of blocks per row
    int numberOfRows; // Number of rows of blocks
    Ball& ball;

public:
    // Constructor for Blocks class
    Blocks(int totalWidth, int numBlocks, int numberOfRows, Ball& ball);

    // Method to get the list of blocks
    std::vector<std::shared_ptr<BlockObject>> getBlocks();

    // Method to populate blocks based on total size and number of blocks
    void populateBlocks();
};
