#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <cxxabi.h> //to be removed


class OrderBook
{
    public:
        OrderBook(std::string filename);
        /** returns a vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
        /** returns a vector of all know timestamps in the dataset*/
        std::vector<std::string> getKnownTimestamps(std::string timestamp);
        /** returns a vector of orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);
        /** returns an amount of the lowest ask or highest bid*/
        static double getOrderAmount(std::vector<OrderBookEntry>& orders, double price);
        
    
        /** returns the earliest time in the orderbook */
        std::string getEarliestTime();
        
        /** returns the next time after the
         *  sent time in the orderbook
         *  If there is no next timestamp, wraps around to the start
         */
        std::string getNextTime(std::string timestamp);
        
        /** inserts an order into the orders vector and sorts it by a timestamp */
        void insertOrder(OrderBookEntry& order);
        
        /** returns a vector of sale orders for a product in aparticular timestamp */
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
       
        /** returns the highest price from the vector of orders  */
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        
        /** returns the lowest price from the vector of orders  */
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        
        /** returns a middle price between a product bid and ask rate  */
        static double getMidPrice(double currentAsk, double currentBid);

    private:
        std::vector<OrderBookEntry> orders;
    
    /** helper function - to be removed in production*/
    std::string demangled(std::string const& sym) {
        std::unique_ptr<char, void(*)(void*)>
            name{abi::__cxa_demangle(sym.c_str(), nullptr, nullptr, nullptr), std::free};
        return {name.get()};
    }

};
