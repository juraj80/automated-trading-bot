#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>

#include "OrderBookEntry.h"
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    // returns the first timestamp
    currentTime = orderBook.getEarliestTime();
    trades = 0;
    printMainMenu();
    input = getMainInput();
    
    if(input==1)
    {
        // inserts the starting currencies to the user wallet
        userWallet.insertCurrency("BTC", 10);
        userWallet.insertCurrency("ETH", 50);
        while(true)
        {
            printUserMenu();
            input = getUserOption();
            processUserOption(input);
        }
    }
    if(input==2)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout<<"Automated trading started" << std::endl << std::endl;
        // inserts the starting currencies to the bot wallet
        botWallet.insertCurrency("BTC", 10);
        botWallet.insertCurrency("ETH", 23);
        
        std::cout<<"\nThe starting balance of the wallet: \n"<<std::endl;
        std::cout<<botWallet.toString()<<std::endl;
        // instance of the MerkelBot
        MerkelBot bot(orderBook, botWallet);
        // iterates over the order book
        do{
            std::cout<<"\ncurrent time is: " << currentTime << std::endl << std::endl;
            // if conditions are met, the bot creates an order
            bot.marketAnalysis(currentTime);
            gotoNextTimeframe();
        } while(currentTime != orderBook.getEarliestTime());
        
        std::cout<<"\nNumber of trades executed: "<<trades<<std::endl;
        std::cout<<"Wallet ending balance:"<<std::endl;
        std::cout<<botWallet.toString()<<std::endl;
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout<<"Execution time: "<<duration.count()<<" milliseconds"<< std::endl;


    }
}

void MerkelMain::printMainMenu()
{
    std::cout << "Choose trading mode:" << std::endl;
    std::cout << "1: Manual Trading" << std::endl;
    std::cout << "2: Automated Trading" << std::endl;
}

void MerkelMain::printUserMenu()
{
    std::cout << "Manual Trading options: " << std::endl << std::endl;
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;
    std::cout << "============== " << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "\nProduct: " << p << std::endl;
        // creates vectors of bids and asks
        std::vector<OrderBookEntry> entriesAsk = orderBook.getOrders(OrderBookType::ask,
                                                                p, currentTime);
        std::vector<OrderBookEntry> entriesBids = orderBook.getOrders(OrderBookType::bid,
        p, currentTime);

        std::cout << "======================= " << std::endl;
        std::cout << "Orders seen: " << entriesAsk.size() << std::endl;
        double minAsk = OrderBook::getLowPrice(entriesAsk); // calculates the min price
        std::cout << "Min ask: " << minAsk << std::endl;
        double maxBid = OrderBook::getHighPrice(entriesBids); // calculates the min price
        std::cout << "Max bid: " << maxBid << std::endl;
        double midPrice = OrderBook::getMidPrice(minAsk, maxBid); // calculates the mid price
        std::cout << "Mid price: " << midPrice << std::endl;
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    // user inputs an ask order
    std::getline(std::cin, input);
    // split up an user input into multiple strings
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            // creates ask entry from the user input
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (userWallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    // user inputs a bid order
    std::getline(std::cin, input);
    // split up an user input into multiple strings
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            // creates bid entry from the user input
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (userWallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << userWallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. \n" << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        // runs matching engine for the current timestamp and returns sales if any
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        trades+=sales.size();
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            salesLog.writeLog(sale.toString());
        
            if (sale.username == "simuser")
            {
                // update the wallet
                userWallet.processSale(sale);
            }
            if (sale.username == "merkelBot")
            {
                // update the wallet
                botWallet.processSale(sale);
                // write a log to a file
                walletLog.writeLog(currentTime);
                walletLog.writeLog(botWallet.toString());
                
            }
        }
    }
    // moves to the next timeframe
    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    } catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl << std::endl;
    return userOption;
}

int MerkelMain::getMainInput()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1 or 2:" << std::endl;
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    } catch(const std::exception& e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }       
}
