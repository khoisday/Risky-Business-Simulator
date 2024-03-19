/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#include "BronzeCompany.h"

BronzeCompany::BronzeCompany() {
    // Initialize bronze company attributes
    companyName = "";
    companyShares = 0;
    companyCost = 0;
    companyLevel = 0;
    companyPower = "";
    companySharePrice = 0;
    companyOwner = "Nobody";
}

BronzeCompany::BronzeCompany(string name, int shares, int cost, int level, int sharePrice, string power, string owner) {
    // Initialize bronze company attributes with provided values
    companyName = name;
    companyShares = shares;
    companyCost = cost;
    companyLevel = level;
    companyPower = power;
    companySharePrice = sharePrice;
    companyOwner = owner;
}

/* Function to set company Data at the start of game based on the level of company*/
void BronzeCompany::setCompanyData(int numberOfPlayers) {
    setCompanyLevel(3); //Company level for silver company is 3
    setCompanyCost(2 * 5 * companyLevel); //Set the cost for silver company using formula
    //Create a random number to calculate the total shares using the formula 10-20 * number of player + company cost
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(10, 20); //Random a number from 10-20
    int randomNumber = dist(gen);
    int totalShares = randomNumber * numberOfPlayers + companyCost;
    setCompanyShares(totalShares);
    setCompanyCost(totalShares); //Set the cost for company takeover
    uniform_int_distribution<int> dist2(1, companyLevel + 4);
    setCompanySharePrice(dist2(gen));
}
