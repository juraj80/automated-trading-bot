#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "MerkelBot.h"


int main()
{   
    MerkelMain app{};
    app.init();
    
    CSVReader::readCSV("20200317.csv");
  
}
