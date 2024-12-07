#pragma once

#include <functional>
#include <map>
#include <thread>
#include <atomic>
#include <iostream>

#ifdef _WIN32
#include <conio.h> // For Windows
#else
#include <unistd.h>
#include <termios.h>
#endif

class Input {
public:
    using KeyCallback = std::function<void()>;  // Define the type for the callback function

private:
    std::map<char, KeyCallback> keyCallbacks;  // Map to store key and their associated callback
    std::atomic<bool> isRunning;  // Atomic flag to control input thread
    std::thread inputThread;  // Thread to listen for key presses

    // Function to listen for key presses
    void listenForKeys();

public:
    // Constructor
    Input();

    // Destructor
    ~Input();

    // Assign a callback function to a key
    void assignCallback(char key, KeyCallback callback);

    // Stop the input thread
    void stopListening();

    // Start the input thread (running in background)
    void startListening();

    // Platform specific non-blocking input function
    char getKey();

    // Platform specific non-blocking input function
    std::thread getThread();
};
