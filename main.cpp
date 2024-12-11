#include "input.h"
#include "object.h"
#include "draw.h"
#include "blocks.h"
#include "ball.h"
#include "bar.h"
#include "manager.h"
#include "running.h"
#include <iostream>
#include <thread>


using namespace std;

int main() {
    float tickTime = 80; // For updating the game state
    int framerate = 60; // For drawing on screen

    // Dimensions
    int rows = 20; int cols = 100;
    Draw draw(rows, cols);

    // Collision manager (semaphore logic)
    CollisionManager manager;

    // Create ball object
    Ball ball({(float) cols/2,
            (float) rows/2},
            1, 1,
            {-1, -1},
            &manager,
            {rows, cols}
            );
    draw.addObject(ball);

    // Thread to check if ball collides with a wall or the bar
    thread ballThread([&ball]() { ball.checkCollision(); });

    // Create the blocks
    int blocksPerRow = 10; int numberOfRows = 3;
    Blocks blocks(cols, rows, blocksPerRow, numberOfRows, ball, &manager, &draw);
    vector<BlockObject*> blockPtrs = blocks.getBlocks();
    draw.addObjects(blockPtrs);

    // Threads that check if the ball collides with a block
    std::vector<std::thread> blockThreads = blocks.getThreads();

    // Create bar
    Bar bar(cols, rows, '-');
    ball.addCollisionObject(bar);
    draw.addObject(bar);

    // Create walls
    CollisionObject top({(float) cols/2, -9}, cols/2, 9, '-', &manager);
    CollisionObject left({-9, (float) rows/2}, 9, rows/2, '|', &manager);
    CollisionObject right({(float) cols+9, (float) rows/2}, 9, rows/2, '|', &manager);
    top.setDraw(false); ball.addCollisionObject(top);
    left.setDraw(false); ball.addCollisionObject(left);
    right.setDraw(false); ball.addCollisionObject(right);

    // Input handling
    Input inputHandler;
    inputHandler.assignCallback('a', [&bar]() { bar.moveHorizontally(-5); });
    inputHandler.assignCallback('d', [&bar]() { bar.moveHorizontally(5); });
    inputHandler.assignCallback('q', []() { exit(0); });
    inputHandler.startListening();

    // Clock tick thread: call onClockTick and update the drawing
    thread updateThread([&ball, &draw, &tickTime]() {
        while (true) {
            if (Running::getInstance().shouldStop()) {
                return;
            }
            ball.onClockTick();
            draw.update();
            this_thread::sleep_for(chrono::milliseconds((int) tickTime));
            // Increases speed gradually
            if (tickTime > 10) {
                tickTime *= 0.995;
            }
        }
    });

    // Drawing thread: draw the updated screen
    thread drawThread([&draw, &framerate]() {
        while (true) {
            if (Running::getInstance().shouldStop()) {
                return;
            }
            draw.draw();
            this_thread::sleep_for(chrono::milliseconds(1000/framerate));
        }
    });

    // Game loop (end game if ball is out of bounds)
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(100));
        int endGame = 0;
        // If ball goes beyond the floor
        if (ball.getCenter()[1] > rows) {
            Running::getInstance().stopThreads();
            cout << "Game over!" << endl;
            endGame = 1;
        }
        // If all blocks are destroyed
        if (draw.getScore() == blocksPerRow*numberOfRows) {
            Running::getInstance().stopThreads();
            cout << "You win!" << endl;
            endGame = 1;
        }
        if (endGame) {
            // Join threads and exit
            inputHandler.stopListening();
            updateThread.join();
            drawThread.join();
            ballThread.join();
            for (auto& t : blockThreads) {
                if (t.joinable()) t.join();
            }
            exit(0);
        }
    }
}
