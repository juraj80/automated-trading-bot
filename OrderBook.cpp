#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>

#include <typeinfo>

OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

std::vector<std::string> OrderBook::getKnownTimestamps(std::string timestamp)
{
    // vector of timestamps strings up to current timestamp
    std::vector<std::string> timestamps;
    // container for storing mapped timestamps
    std::map<std::string,bool> timeMap;
    for (OrderBookEntry& e : orders)
    {
        timeMap[e.timestamp] = true;
    }
    // now flatten the map to a vector of strings
    for (auto const& e : timeMap) // change the logic to filter only up to relevant timestamp
    {
        if (e.first < timestamp) timestamps.push_back(e.first);
    }
    return timestamps;
}



std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

double OrderBook::getOrderAmount(std::vector<OrderBookEntry>& orders, double price)
{
    // checks orders and if the price is met
    // returns entry amount
    for(OrderBookEntry& e: orders)
    {
        if(e.price == price) return e.amount;
    }
    return 0;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max) max = e.price;
    }
    return max;     // returns the highest price
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;    // returns the lowest price

}


double OrderBook::getMidPrice(double currentAsk, double currentBid)
{
    return (currentAsk + currentBid)/2; // returns the mid price
}


std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp; // returns the first timestamp
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    // compares timestamps and returns next in row
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    // enters an order, then sorts the vector
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}


std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);

    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                    timestamp);
    std::vector<OrderBookEntry> sales;

    // a little check to ensure we have bids and asks to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << "OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;
    
    for (OrderBookEntry& ask : asks)
    {
        for (OrderBookEntry& bid : bids)
        {
            // we have a match,
            // we create a new sale entry
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale{ask.price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (bid.username == "merkelBot")
                {
                    sale.username = "merkelBot";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }
                if (ask.username == "merkelBot")
                {
                    sale.username = "merkelBot";
                    sale.orderType =  OrderBookType::asksale;
                }
                
            
                // works out how much was sold and
                // creates new bids and asks covering
                // anything that was not sold
                if (bid.amount == ask.amount) // bid completely clears ask
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0; // make sure the bid is not processed again
                    break; // go onto the next ask
                }
                if (bid.amount > ask.amount) // ask is completely gone slice the bid
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    // we adjust the bid in place
                    // so it can be used to process the next ask
                    bid.amount =  bid.amount - ask.amount;
                    break; // ask is completely gone, so go to next ask
                }

                if (bid.amount < ask.amount &&
                   bid.amount > 0) // bid is completely gone, slice the ask
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    // updates the ask
                    // and allows further bids to process the remaining amount
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0; // make sure the bid is not processed again
                    continue; // some ask remains so go to the next bid
                }
            }
        }
    }
    return sales;             
}
