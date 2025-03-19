#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <fstream>
#include <iostream>

//extern std::ofstream logFile;

std::ofstream& logFile();
void deinitLogger();

#endif