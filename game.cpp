#include "game.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

// Constructor to initialize the game with screen size
Game::Game(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      ball({screenWidth / 2, screenHeight / 2}, 2, 2, {1, 1}),
      bar(screenWidth, screenHeight, '='), blocks(screenWidth, 10, 5), draw(screenHeight, screenWidth), input(),
      gameRunning(true) {

    initialize();
}

// Destructor to join threads and clean up
Game::~Game() {
    gameRunning = false;

    // Join input and drawing threads
    if (inputThread.joinable()) inputThread.join();
    if (drawThread.joinable()) drawThread.join();

    // Join collision check threads
    for (auto& t : collisionThreads) {
        if (t.joinable()) t.join();
    }
}

// Initialize the game state (create objects, set up threads)
void Game::initialize() {
    // Populate blocks
    blocks.populateBlocks();

    // Set up all collision objects
    allCollisionObjects.push_back(&ball);

    auto blockObjects = blocks.getBlocks();
    for (auto& block : blockObjects) {
        allCollisionObjects.push_back(&block);
    }

    // Set up input thread
    input.startListening();
    inputThread = input.getThread();

    // Set up draw thread
    drawThread = std::thread([this]() { drawGame(); });

    // Set up collision check threads
    for (auto& obj : allCollisionObjects) {
        collisionThreads.push_back(std::thread([this, obj]() { checkCollisions(*obj); }));
    }
}

// Game loop, updates every tick (0.1s)
void Game::gameLoop() {
    while (gameRunning) {
        // Wait for the next tick
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Update game state
        updateGame();
    }
}

// Function to handle user input
void Game::handleInput() {
    input.assignCallback('a', [this]() { bar.moveHorizontally(-1); });
    input.assignCallback('d', [this]() { bar.moveHorizontally(1); });
}

// Function to check collisions for each object in a separate thread
void Game::checkCollisions(CollisionObject& object) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        object.checkCollision();  // Check for collisions
    }
}

// Function to update the game state
void Game::updateGame() {
    ball.move();  // Move the ball
}

// Function to draw the game state
void Game::drawGame() {
    draw.clear();  // Clear the screen
    vector<Object> objects = {ball, bar};
    draw.setObjects(objects);  // Set the objects to draw
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        draw.update();  // Update the screen based on objects' positions
        draw.draw();   // Draw the updated screen
    }
}

// Start the game
void Game::start() {
    gameLoop();  // Start the game loop
}
