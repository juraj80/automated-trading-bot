#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

#include "Wallet.h"
#include "MerkelBot.h"
#include "Logger.h"



class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
        OrderBook orderBook{"/Users/jurajklucka/Documents/SCHOOL/CS/Semester_3/Object-Oriented-Programming/midterm/BetaProject/MerkelBot/20200317.csv"};
    private:
        void printMainMenu();
        void printUserMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        int getMainInput();
        void processUserOption(int userOption);

        std::string currentTime;
        double trades;
        
        Wallet userWallet;
        Wallet botWallet;
        Logger salesLog{"salesLog.csv"};
        Logger walletLog{"walletLog.csv"};


};
