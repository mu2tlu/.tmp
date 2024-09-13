#pragma once

#include <csignal>
#include <iostream>

class SignalHandler {
    
    private:
        static void handler(int signum);
        static sig_atomic_t running; //

    public:
        static void setup();
        static bool isRunning();
};