#include "blocks.h"
#include <thread>

using namespace std;

// Constructor for Blocks
Blocks::Blocks(int totalWidth, int numBlocks, int numberOfRows, Ball& ball)
    : totalWidth(totalWidth), numBlocks(numBlocks), numberOfRows(numberOfRows), ball(ball) {
    populateBlocks();
    std::vector<std::thread> threads;
    for (auto& block : blocks) {
        block->addCollisionObject(ball);
        threads.push_back(std::thread([&block]() { block->checkCollision(); }));
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
    int blockWidth = totalWidth / numBlocks;
    int blockHeight = blockWidth / 2;  // Height is half the width of the block

    // Calculate block positions and create BlockObject instances
    for (int row = 0; row < numberOfRows; ++row) {
        for (int col = 0; col < numBlocks; ++col) {
            // Calculate the center of each block
            array<float, 2> center = {
                static_cast<float>(blockWidth * col + blockWidth / 2.0),
                static_cast<float>(blockHeight * row + blockHeight / 2.0)
            };

            char symbol = '#';  // Symbol representing the block
            auto block = std::make_shared<BlockObject>(center, blockWidth / 2 - 1, blockHeight / 2 - 1, symbol);
            blocks.push_back(std::move(block));  // Move the shared_ptr into the vector
        }
    }
}

// Get the list of blocks
vector<std::shared_ptr<BlockObject>> Blocks::getBlocks() {
    return blocks;
}
