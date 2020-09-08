//
//  MerkelBot.cpp
//  MerkelBot
//
//  Created by Juraj Klucka on 16/06/2020.
//  Copyright © 2020 Juraj Klucka. All rights reserved.
//

#include "MerkelBot.h"
#include <iostream>
#include <numeric>

MerkelBot::MerkelBot(OrderBook& _orderBook, Wallet& _wallet):orderBook(_orderBook),wallet(_wallet)
{

}

double MerkelBot::getAvgPrice(std::vector<double> prices)
{
    return std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
}

double MerkelBot::getSMA(std::string timestamp, std::string product)
{
    // vector of mid prices for particular product from previous timestamps
    std::vector<double> midPrices;
    
    // iterates over timestamps
    for (std::string const& t :orderBook.getKnownTimestamps(timestamp))
    {
        // ask and bid entries vectors for that particular timestamp
        std::vector<OrderBookEntry> entriesAsk = orderBook.getOrders(OrderBookType::ask,product, t);
        std::vector<OrderBookEntry> entriesBid = orderBook.getOrders(OrderBookType::bid,product, t);
        // calculates the mid price from the lowest ask and highest bid
        // and pushes it into the midPrices vector
        double midPrice = 0;
        if(entriesAsk.size() > 0 && entriesBid.size() > 0)
        {
            double minAsk = OrderBook::getLowPrice(entriesAsk);
            double maxBid = OrderBook::getHighPrice(entriesBid);
            midPrice = OrderBook::getMidPrice(minAsk, maxBid);
        }
        midPrices.push_back(midPrice);
    }
    // returns the average mid price from previous timestamps
    return getAvgPrice(midPrices);
}

void MerkelBot::marketAnalysis(std::string timestamp)
{
    // iterates over products in a given timestamp
    for (std::string const& p :orderBook.getKnownProducts())
    {
        // an average mid price for particular product calculated from previous timestamps
        double avgMidPrice = getSMA(timestamp,p);
        std::cout << "average Mid Price for product " << p <<" is " << avgMidPrice << std::endl;
        
        // vector of current ask orders for the product p
        std::vector<OrderBookEntry> entriesAsk = orderBook.getOrders(OrderBookType::ask,p, timestamp);
        std::vector<OrderBookEntry> entriesBid = orderBook.getOrders(OrderBookType::bid,p, timestamp);

        std::cout << "======================= " << std::endl;
        std::cout << "Orders seen: " << entriesAsk.size() << std::endl;
    
        if(entriesAsk.size()>0 && entriesBid.size()>0)
        {
            double minAsk = OrderBook::getLowPrice(entriesAsk);
            std::cout << "Min ask: " << minAsk << std::endl;
            double maxBid = OrderBook::getHighPrice(entriesBid);
            std::cout << "Max bid: " << maxBid << std::endl;

            // MARKET ANALYSIS SECTION
            // returns current mid price for a product from the current timestamp
            double midPrice = OrderBook::getMidPrice(minAsk, maxBid);
            std::cout << "actual mid-price for product " << p <<" is " << midPrice << std::endl;
            
            if(midPrice < avgMidPrice)
            {
                // calculates a bid order amount
                double amount = OrderBook::getOrderAmount(entriesAsk, minAsk);
                // creates a bid order and puts it in the order book
                generateBid(minAsk, amount, timestamp, p);
            }   
            else if(midPrice > avgMidPrice)
            {
                // calculates an ask order amount
                double amount = OrderBook::getOrderAmount(entriesBid, maxBid);
                // creates an ask order and puts it in the order book
                generateAsk(maxBid, amount, timestamp, p);
            }
        }
    }
}

void MerkelBot::generateBid(double price, double amount, std::string timestamp, std::string product)
{
    try {
        OrderBookEntry obe(price,
                           amount,
                           timestamp,
                           product,
                           OrderBookType::bid);
        
        obe.username = "merkelBot";
        
        if (wallet.canFulfillOrder(obe)) // the bot wallet has the currency
        {
            std::cout << "Wallet looks good. " << std::endl;
            orderBook.insertOrder(obe);
            ordersLog.writeLog(obe.toString());
        }
        else
        {
            std::cout << "Wallet has insufficient funds . " << std::endl;
        }
    } catch (const std::exception& e)
    {
        std::cout << " MerkelBot::generateBid Bad input " << std::endl;
    }
}

void MerkelBot::generateAsk(double price, double amount, std::string timestamp, std::string product)
{
    try {
        OrderBookEntry obe(price,
                           amount,
                           timestamp,
                           product,
                           OrderBookType::ask);
        
        obe.username = "merkelBot";
        
        if (wallet.canFulfillOrder(obe))
        {
            std::cout << "Wallet looks good. " << std::endl;
            orderBook.insertOrder(obe);
            ordersLog.writeLog(obe.toString());

        }
        else
        {
            std::cout << "Wallet has insufficient funds . " << std::endl;
            
        }
    } catch (const std::exception& e)
    {
        std::cout << " MerkelBot::generateBid Bad input " << std::endl;
    }
    
}
