#include "logger.hpp"


//std::ofstream logFile("log.txt", std::ios::app);

std::ofstream& logFile() {
    // This static variable is initialized only once on the first call.
    static std::ofstream log("log.txt", std::ios::app);
    if (!log.is_open()) {
        throw std::runtime_error("Failed to open log file!");
    }
    return log;
}

void deinitLogger(){
    logFile().close();
}