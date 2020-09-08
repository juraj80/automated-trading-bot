#include "Logger.h"
#include <fstream>


Logger::Logger(std::string fileName)
{
    // sets the log file path
    std::string path = "./logs/" + fileName;
    // creates the log file
    mStream.open(path);
    if (mStream.fail())
    {
        throw std::exception{};
    }
}

Logger::~Logger()
{
    mStream.close();
}

void Logger::writeLog(const std::string& message)
{
    // writes the message into the log file
    mStream << message <<"\n";
}


