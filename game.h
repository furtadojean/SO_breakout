#pragma once

#include "object.h"
#include "bar.h"
#include "ball.h"
#include "blocks.h"
#include "draw.h"
#include "input.h"
#include <vector>
#include <thread>
#include <semaphore.h>
#include <atomic>

class Game {
private:
    int screenWidth, screenHeight;  // Screen size
    Ball ball;                      // Ball object
    Bar bar;                        // Bar object
    Blocks blocks;                  // Blocks collection
    Draw draw;                      // Draw handler
    Input input;                    // Input handler

    std::vector<CollisionObject*> allCollisionObjects; // All collision objects for the game
    std::atomic<bool> gameRunning;  // Flag to control the game loop

    sem_t semaphore;  // Semaphore to synchronize threads

    // Threads for handling input, drawing, and checking collisions
    std::thread inputThread;
    std::thread drawThread;
    std::vector<std::thread> collisionThreads;

    // Function to initialize the game
    void initialize();

    // Function to update the game state every tick
    void gameLoop();

    // Function to handle user input (keyboard input)
    void handleInput();

    // Function to check collisions for each object in a separate thread
    void checkCollisions(CollisionObject& object);

    // Function to update the game state (move objects, check collisions)
    void updateGame();

    // Function to draw the game state
    void drawGame();

public:
    // Constructor to initialize the game with screen size
    Game(int screenWidth, int screenHeight);

    // Destructor to join threads and clean up
    ~Game();

    // Function to start the game
    void start();
};
