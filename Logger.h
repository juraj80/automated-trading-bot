#pragma once
#include <fstream>

class Logger
{
    public:
        Logger(std::string outfile);
        ~Logger();
                
        /**writes a single message to a log file*/
        void writeLog(const std::string& message);
    
    std::ofstream mStream;

};

