#include "input.h"
#include "object.h"
#include "draw.h"
#include "blocks.h"
#include "ball.h"
#include "bar.h"
#include <iostream>
#include <thread>



using namespace std;

void my_thread() {
    //CollisionObject ball({5, 5}, 2, 2, 'O');
    //cout << "Ball center: " << ball.getCenter()[0] << ", " << ball.getCenter()[1] << endl;
    //cout << "Ball width: " << ball.getHWidth()*2 << endl;
    //cout << "Ball height: " << ball.getHHeight()*2 << endl;
    //cout << "Ball symbol: " << ball.getSymbol() << endl;
    //cout << "Ball has physics: " << ball.getHasPhysics() << endl;
    //cout << "Ball collision active: " << ball.isCollisionActive() << endl;

    int rows = 20; int cols = 50;
    Draw draw(rows, cols);
    //draw.setObjects({ball});


    std::shared_ptr<Ball> ballPtr = std::make_shared<Ball>(Ball({(float) cols/2, (float) rows/2}, 1, 1, {-1, -1}));
    Ball ball({(float) cols/2, (float) rows/2}, 1, 1, {-1, -1});
    draw.addObject(ballPtr);
    //checkCollision
    thread([&ball]() { ball.checkCollision(); }).detach();

    Blocks blocks(cols, 10, rows/5, ball);

    Bar bar(cols, rows, '-');
    ball.addCollisionObject(bar);
    //draw.addObject(std::move(bar));

    CollisionObject top({(float) cols/2, 0}, cols/2, 1, '-');
    CollisionObject bottom({(float) cols/2, (float) rows}, cols/2, 1, '-');
    CollisionObject left({-1, (float) rows/2}, 1, rows/2, '|');
    CollisionObject right({(float) cols, (float) rows/2}, 1, rows/2, '|');
    //draw.addObject(std::move(top));
    top.setDraw(true); ball.addCollisionObject(top);
    bottom.setDraw(false); ball.addCollisionObject(bottom);
    left.setDraw(false); ball.addCollisionObject(left);
    right.setDraw(false); ball.addCollisionObject(right);

    while (true) {
        ball.onClockTick();
        //cout << "Ball center: " << ball.getCenter()[0] << ", " << ball.getCenter()[1] << endl;
        draw.update();
        draw.draw();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
int main() {
    thread(my_thread).join();
}










// Function to handle when the 'W' key is pressed
void onWPressed() {
    std::cout << "W key pressed!" << std::endl;
}

// Function to handle when the 'A' key is pressed
void onAPressed() {
    std::cout << "A key pressed!" << std::endl;
}

int main1() {
    // Create an Input object
    Input inputHandler;

    // Assign callbacks to keys
    inputHandler.assignCallback('w', onWPressed);
    inputHandler.assignCallback('a', onAPressed);

    // Start the input listener on a separate thread
    inputHandler.startListening();

    // Main loop: while input is being handled in a separate thread
    std::cout << "Press 'w' or 'a' to trigger the callbacks." << std::endl;
    std::cout << "Press 'q' to quit." << std::endl;

    while (true) {
        // Main program logic can go here
        std::cout << "Main program running..." << std::endl;
        // Sleep
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    char quitKey = ' ';
    while (quitKey != 'q') {
        quitKey = getchar();  // Wait for 'q' to quit the program
    }

    // Stop listening for input when done
    inputHandler.stopListening();
    std::cout << "Program finished." << std::endl;

    return 0;
}

