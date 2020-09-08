#pragma once

#include <string>

enum OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry
{
    public:
        OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType, 
                        std::string username = "dataset");

        /** a function that performs the conversion from a string to an enum type*/
        static OrderBookType stringToOrderBookType(std::string s);
       
        /** a function that performs the conversion of an enum type to a string */
        std::string OrderBookTypeToString();
        
        /** returns a string representation of an OrderBookEntry object */
        std::string toString();
    
        /** returns true if the first object's timestamp is less than the second object's timestamp  */
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.timestamp < e2.timestamp;
        }
    
        /** returns true if the first object's price is less than the second object's price  */
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price < e2.price;
        }
    
        /** returns true if the first object's price is higher than the second object's price  */
         static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price > e2.price;
        }

        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;
};
