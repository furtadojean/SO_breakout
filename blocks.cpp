#include "blocks.h"
#include <thread>

using namespace std;

void BlockObject::onCollision(Object& object) {
    // Handle the collision logic here
    getCollisionManager()->acquireSemaphore();  // Protect critical section with semaphore
    if(!getDraw()) {
        getCollisionManager()->releaseSemaphore();
        return;
    }
    setCollisionActive(false);
    setDraw(false);
    draw->increaseScore();

    // Wait for collision to end
    getCollisionManager()->waitForEndCollision(&object);

    // End collision, allowing other threads to proceed
    getCollisionManager()->releaseSemaphore();
}

// Constructor for Blocks
Blocks::Blocks(int totalWidth, int totalHeight, int blocksPerRow, int numberOfRows, Ball& ball, CollisionManager* manager, Draw* draw)
    : totalWidth(totalWidth), totalHeight(totalHeight), blocksPerRow(blocksPerRow), numberOfRows(numberOfRows), ball(ball), manager(manager), draw(draw) {
    populateBlocks();
    for (auto& block : blocks) {
        block.addCollisionObject(ball);
        threads.push_back(std::thread([&block]() { block.checkCollision(); }));
    }
    // Join all threads
    for (auto& t : threads) {
        if (t.joinable())
            t.detach();
    }
}

// Populate blocks based on total size and number of blocks
void Blocks::populateBlocks() {
    blocks.clear();

    // Calculate the width for each block based on the total size
    int blockWidth = totalWidth / blocksPerRow;
    int blockHeight = totalHeight / 3 / numberOfRows;

    // Calculate block positions and create BlockObject instances
    for (int row = 0; row < numberOfRows; ++row) {
        for (int col = 0; col < blocksPerRow; ++col) {
            // Calculate the center of each block
            array<float, 2> center = {
                static_cast<float>(blockWidth * col + blockWidth / 2.0),
                static_cast<float>(blockHeight * row + blockHeight / 2.0)
            };

            char symbol = '#';  // Symbol representing the block
            BlockObject block(center, blockWidth / 2-1, blockHeight / 2-1, symbol, manager, draw);
            blocks.push_back(block);
        }
    }
}

// Get the list of blocks
vector<BlockObject*> Blocks::getBlocks() {
    std::vector<BlockObject*> blockPtrs;

    for (auto& block : blocks) {
        blockPtrs.push_back(&block); // Add pointer to the vector
    }

    return blockPtrs;
}
