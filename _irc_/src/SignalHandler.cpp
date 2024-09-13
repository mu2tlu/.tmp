#include "../include/SignalHandler.hpp"


sig_atomic_t SignalHandler::running = 1;

void SignalHandler::setup(){
    std::signal(SIGINT, SignalHandler::handler); // ctrl c exit signal
    std::signal(SIGTERM, SignalHandler::handler);
    std::signal(SIGQUIT,SignalHandler::handler);
}

void SignalHandler::handler(int signum){ 
    
    
    running = 0;
    std::cout << "Received Signal " << signum << ".Shutting Down..." << std::endl;
}

bool SignalHandler::isRunning(){ 
    return running;
}
