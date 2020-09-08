#include "OrderBookEntry.h"
#include <iostream>

OrderBookEntry::OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType, 
                        std::string _username)
: price(_price), 
  amount(_amount), 
  timestamp(_timestamp),
  product(_product), 
  orderType(_orderType), 
  username(_username)
{
    
}

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  // checks the string s and returns the enum type representation
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }
  return OrderBookType::unknown;
}

std::string OrderBookEntry::OrderBookTypeToString()
{
    // checks the enum type and returns the string representation
    switch (orderType)
    {
        case bid: return "bid";
        case ask: return "ask";
        case asksale: return "asksale";
        case bidsale: return "bidsale";
        default: return "unknown";
    }
}

std::string OrderBookEntry::toString()
{
    // converts an entry properties to the string representation
    std::string orderType = OrderBookTypeToString();
    return timestamp + std::to_string(price) + "," + std::to_string(amount)+ "," +product+ "," +orderType;
}


