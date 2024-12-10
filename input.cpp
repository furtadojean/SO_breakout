#include "input.h"
#include <iostream>
#include <thread>
#include <map>
#include <atomic>
#include "running.h"

#ifdef _WIN32
#include <conio.h>  // Windows-specific header for _kbhit() and _getch()
#else
#include <unistd.h>
#include <termios.h>
#endif

using namespace std;

// Constructor
Input::Input() : isRunning(false) {}

// Destructor
Input::~Input() {
    stopListening();
}

// Function to listen for key presses
void Input::listenForKeys() {
    while (isRunning) {
        if (Running::getInstance().shouldStop()) {
            isRunning = false;
            return;
        }
        char key = getKey();  // Get a key (non-blocking)

        if (key != '\0') {  // If a key is pressed
            // If a callback exists for the key, call it
            if (keyCallbacks.find(key) != keyCallbacks.end()) {
                keyCallbacks[key]();  // Trigger the callback for the pressed key
            }
        }
    }
}

// Assign a callback function to a key
void Input::assignCallback(char key, KeyCallback callback) {
    keyCallbacks[key] = callback;  // Store the callback for the key
}

// Start the input thread (non-blocking)
void Input::startListening() {
    if (isRunning) {
        return;  // If already running, return
    }
    isRunning = true;
    inputThread = std::thread(&Input::listenForKeys, this);  // Start the thread
}

// Stop the input thread
void Input::stopListening() {
    isRunning = false;
    if (inputThread.joinable()) {
        inputThread.join();  // Ensure the thread finishes before exiting
    }
}

std::thread Input::getThread() {
    return std::move(inputThread);
}

// Platform-specific non-blocking input function (getKey)
char Input::getKey() {
    char key = '\0';

#ifdef _WIN32
    if (_kbhit()) {  // Check if a key is pressed
        key = _getch();  // Get the pressed key
    }
#else
    struct termios oldt, newt;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echoing
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    newt.c_cc[VMIN] = 1;               // Minimum number of characters to read
    newt.c_cc[VTIME] = 0;              // No timeout

    // Apply new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read a single character
    key = getchar();

    // Restore old terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    return key;
}

