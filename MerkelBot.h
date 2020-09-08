#pragma once

#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "MerkelMain.h"
#include "Wallet.h"
#include "Logger.h"
#include <iostream>

class MerkelBot
{
    public:
        MerkelBot(OrderBook& _orderBook, Wallet& _wallet); // passing reference in a constructor
        
        /** calculates the average price from the vector of order prices */
        static double getAvgPrice(std::vector<double> prices);
        
        /** calculates the moving average of the mid prices from previous timestamps */
        double getSMA(std::string timestamp, std::string product);
        
        /** market analysis for the current timestamp */
        void marketAnalysis(std::string timestamp);
        
        /** creates a bid market order and inserts it into the orderbook */
        void generateBid(double price, double amount, std::string timestamp, std::string product);
        
        /** creates an ask market order and inserts it into the orderbook */
        void generateAsk(double price, double amount, std::string timestamp, std::string product);
        
    private:
        void createLog();
        
        OrderBook& orderBook; 
        Wallet& wallet;
        Logger ordersLog{"ordersLog.csv"};

};



    

