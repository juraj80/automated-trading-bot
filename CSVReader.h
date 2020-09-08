#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>


class CSVReader
{
    public:
        CSVReader();
        /**
         * reads  a CSV file line by line and converts the data into the OrderBookEntry object
         * and returns the vector of OrderBookEntry objects
         */
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        
        /**
         * reads a string line and splits it into the single strings
         * and returns the vector of strings tokenized from the line
         */
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        
        /**
         * converts string variables for price and amount into doubles
         * and returns the OrderBookEntry object generated from the input arguments
         */
        static OrderBookEntry stringsToOBE(std::string price,
                                           std::string amount,
                                           std::string timestamp,
                                           std::string product,
                                           OrderBookType OrderBookType);
    private:
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
     
};
