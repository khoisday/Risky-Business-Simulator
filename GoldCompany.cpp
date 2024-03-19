/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#include "GoldCompany.h"

GoldCompany::GoldCompany() {
    // Initialize gold company attributes
    companyName = "";
    companyShares = 0;
    companyCost = 50;
    companyLevel = 5;
    companyPower = "";
    companySharePrice = 0;
    companyOwner = "Nobody";
}

GoldCompany::GoldCompany(string name, int shares, int cost, int level, int sharePrice, string power, string owner) {
    // Initialize gold company attributes with provided values
    companyName = name;
    companyShares = shares;
    companyCost = cost;
    companyLevel = level;
    companyPower = power;
    companySharePrice = sharePrice;
    companyOwner = owner;
}

/* Function to set company Data at the start of game based on the level of company*/
void GoldCompany::setCompanyData(int numberOfPlayers) {
    setCompanyLevel(5); //Company level for silver company is 5
    setCompanyCost(2 * 5 * companyLevel); //Set the cost for silver company using formula
    //Create a random number to calculate the total shares using the formula 10-20 * number of player + company cost
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(10, 20); //Random a number from 10-20
    int randomNumber = dist(gen);
    int totalShares = randomNumber * numberOfPlayers + companyCost;
    setCompanyShares(totalShares); // Set company cost using the formula
    setCompanyCost(totalShares); //Set the cost for silver company using formula
    //set the shareprice from range 1 - company level + 4 using random number
    uniform_int_distribution<int> dist2(1, companyLevel + 4);
    setCompanySharePrice(dist2(gen)); //Set company using that random number
}