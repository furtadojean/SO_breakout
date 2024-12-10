#include "input.h"
#include "object.h"
#include "draw.h"
#include "blocks.h"
#include "ball.h"
#include "bar.h"
#include "manager.h"
#include <iostream>
#include <thread>



using namespace std;

int main() {
    //CollisionObject ball({5, 5}, 2, 2, 'O');
    //cout << "Ball center: " << ball.getCenter()[0] << ", " << ball.getCenter()[1] << endl;
    //cout << "Ball width: " << ball.getHWidth()*2 << endl;
    //cout << "Ball height: " << ball.getHHeight()*2 << endl;
    //cout << "Ball symbol: " << ball.getSymbol() << endl;
    //cout << "Ball has physics: " << ball.getHasPhysics() << endl;
    //cout << "Ball collision active: " << ball.isCollisionActive() << endl;

    int rows = 20; int cols = 100;
    Draw draw(rows, cols);
    //draw.setObjects({ball});

    CollisionManager manager;

    Ball ball({(float) cols/2,
            (float) rows/2},
            1, 1,
            {-1, -1},
            &manager,
            {rows, cols}
            );
    draw.addObject(ball);
    //checkCollision
    thread ballThread([&ball]() { ball.checkCollision(); });

    Blocks blocks(cols, rows, 10, 3, ball, &manager, &draw);
    vector<BlockObject*> blockPtrs = blocks.getBlocks();
    draw.addObjects(blockPtrs);
    std::vector<std::thread> blockThreads = blocks.getThreads();

    Bar bar(cols, rows, '-');
    ball.addCollisionObject(bar);
    draw.addObject(bar);

    CollisionObject top({(float) cols/2, -9}, cols/2, 9, '-', &manager);
    //CollisionObject bottom({(float) cols/2, (float) rows+9}, cols/2, 9, '-', &manager);
    CollisionObject left({-9, (float) rows/2}, 9, rows/2, '|', &manager);
    CollisionObject right({(float) cols+9, (float) rows/2}, 9, rows/2, '|', &manager);
    //draw.addObject(top);
    //draw.addObject(left);
    //draw.addObject(top);
    //draw.addObject(top);
    top.setDraw(true); ball.addCollisionObject(top);
    //bottom.setDraw(false); ball.addCollisionObject(bottom);
    left.setDraw(false); ball.addCollisionObject(left);
    right.setDraw(false); ball.addCollisionObject(right);

    Input inputHandler;
    // Assign callbacks to keys to move the bar
    inputHandler.assignCallback('a', [&bar]() { bar.moveHorizontally(-5); });
    inputHandler.assignCallback('d', [&bar]() { bar.moveHorizontally(5); });
    inputHandler.assignCallback('q', []() { exit(0); });
    inputHandler.startListening();

    //while (true) {
    //    ball.onClockTick();
    //    //cout << "Ball center: " << ball.getCenter()[0] << ", " << ball.getCenter()[1] << endl;
    //    draw.update();
    //    draw.draw();
    //    this_thread::sleep_for(chrono::milliseconds(10));
    //}

    float tickTime = 80;
    int framerate = 60;
    // Clock tick thread: call onClockTick and update the drawing
    thread updateThread([&ball, &draw, &tickTime]() {
        while (true) {
            ball.onClockTick();
            draw.update();
            this_thread::sleep_for(chrono::milliseconds((int) tickTime));
            if (tickTime > 10) {
                tickTime *= 0.999;
            }
        }
    });

    // Drawing thread: draw the updated screen
    thread drawThread([&draw, &framerate]() {
        while (true) {
            draw.draw();
            this_thread::sleep_for(chrono::milliseconds(1000/framerate));
        }
    });

    while (true) {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Ball center: " << ball.getCenter()[0] << ", " << ball.getCenter()[1] << endl;
        // If ball goes past the floor
        if (ball.getCenter()[1] > rows) {
            cout << "Game over!" << endl;
            exit(0);
            inputHandler.stopListening();
            updateThread.join();
            drawThread.join();
            ballThread.join();
            for (auto& t : blockThreads) {
                if (t.joinable()) t.join();
            }
            std::terminate();
        }
    }
    exit(0);
    std::terminate();
}
