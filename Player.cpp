//
// Created by nguye on 9/29/2023.
//
#include "Player.h"

//Constructors
Player::Player() {
    // Initialize player attributes
    playerName = "";
    playerMoney = 0;
    companiesOwned = 0;
    playerShares = 0;
    powerUsesLeft = 1;
    companiesPortfolio.clear();
}

Player::Player(string name, int money, int numberOwn, int shares, int powerLeft) {
    // Initialize player attributes with provided values
    setPlayerName(name);
    setPlayerMoney(money);
    setCompaniesOwned(numberOwn);
    setPlayerShares(shares);
    setPowerUsesLeft(powerLeft);
    companiesPortfolio.clear();
}

//Accessor Functions
/* Function to return player's name*/
string Player::getPlayerName() {
    return playerName;
}

/* Function to return player money*/
int Player::getPlayerMoney() {
    return playerMoney;
}

/* Function to return number of companies owned*/
int Player::getPlayerShares() {
    return playerShares;
}

/* Function to return number of companies owned*/
int Player::getCompaniesOwned() {
    return companiesOwned;
}

/* Function to return number of power use left*/
int Player::getPowerUsesLeft() {
    return powerUsesLeft;
}

/* Function to return companies portfolio*/
vector<Company *> Player::getCompaniesPortfolio() {
    return companiesPortfolio;
}

/* Function to return player details*/
string Player::getDetails() {
    stringstream details; //Create a formated string
    details << "Name: " << getPlayerName() << endl;
    details << "Money: " << getPlayerMoney() << endl;
    details << "Number of companies owned: " << getCompaniesOwned() << endl;
    details << "Number of share: " << getPlayerShares() << endl;
    details << "Number of power left: " << getPowerUsesLeft() << endl;
    details << "Companies player have shares: " << endl;
    for (int i = 0; i < getCompaniesPortfolio().size(); i++)
        details << getCompaniesPortfolio()[i]->getCompanyName() << endl; //ADD LATER IN COMPANY CLASS
    return details.str(); // return the formated string
}

/* Function to return player details*/
string Player::getPlayerInfo() {
    stringstream playerDetails; //Create a formated string
    playerDetails.clear();
    playerDetails << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    playerDetails << "\t " << playerName << "'s Share Portfolio and Assets\n";
    playerDetails << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    playerDetails << " Total Money: $" << playerMoney << "  Companies Owned: " << companiesOwned << "  Total Shares: "
                  << playerShares << endl;
    if (companiesPortfolio.size() == 0) {
        playerDetails << " Your Share Portfolio is empty, " << playerName << endl;
    } else {
        playerDetails << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        playerDetails << "\tCompany Names         Shares    Power\n";
        for (Company *myCompany: companiesPortfolio) {
            playerDetails << "\t" << myCompany->getCompanyName() << "\t" << myCompany->getCompanyShares() << "\t"
                          << myCompany->getCompanyPower() << endl;
        }
    }
    return playerDetails.str();// return the formated string
}

//Mutator Functions
/* Function to set player name*/
void Player::setPlayerName(string name) {
    playerName = name;
}

/* Function to set player money*/
void Player::setPlayerMoney(int money) {
    playerMoney = money;
}

/* Function to set player number of company own*/
void Player::setCompaniesOwned(int numberOwn) {
    if (numberOwn < 0) // Check if the company owned is < 0 or not
        numberOwn = 0;
    companiesOwned = numberOwn;
}

/* Function to set player shares*/
void Player::setPlayerShares(int shares) {
    if (shares < 0) // Check if the share owned is < 0 or not
        shares = 0;
    playerShares = shares;
}

/* Function to set player power use left*/
void Player::setPowerUsesLeft(int powerLeft) {
    if (powerLeft < 0) // Check if the power player can user is <0 or not
        powerLeft = 6;
    powerUsesLeft = powerLeft;
}

/* Function to pushback company to the company list */
void Player::setCompaniesPortfolio(Company *company) {
    companiesPortfolio.push_back(company);
}

/* Function to update companies portfolio
    Paramaters: company: the company will be added to the companies portfolio or delete
                numShares: the number of shares in that company*/
void Player::updateCompaniesPortfolio(Company *company, int numShares) {
    // Initialize a flag to check if the company share is already owned by the player.
    bool companyShareOwned = false;
    // Loop through the player's companies portfolio.
    for (int i = 0; i < companiesPortfolio.size(); i++) {
        //If that company name already in the list just update the shares
        if (company->getCompanyName() == companiesPortfolio[i]->getCompanyName()) {
            companyShareOwned = true;
            companiesPortfolio[i]->updateCompanyShares(numShares);
            //If shares equal zero and the power is No(that player don't own the company). Erase it from the list
            if ((companiesPortfolio[i]->getCompanyShares() == 0) &&
                (companiesPortfolio[i]->getCompanyPower() == "No")) {
                companiesPortfolio.erase(companiesPortfolio.begin() + i);
            }
            break;
        }
    }
    int companyIndex = -1;
    //If player don't own that company share then create a new instance for that company then check for the alphabetically order
    //Then add it in the correct order (alphabetically)
    if (!companyShareOwned) {
        string companyName = company->getCompanyName();
        int companyLevel = company->getCompanyLevel();
        int companyCost = company->getCompanyCost();
        int sharePrice = company->getCompanySharePrice();
        Company *userShareCompany = new Company(companyName, numShares, companyCost, companyLevel, sharePrice, "No",
                                                "Nobody"); //Create new instance for the company after get all the need
        for (int i = 0; i < companiesPortfolio.size(); i++) {
            //Find the clost company that have name > than the company we want to add (alphabettically comparision)
            if (companiesPortfolio[i]->getCompanyName() > company->getCompanyName()) {
                companyIndex = i;
                break; //if found break it
            }
        }
        if (companyIndex == -1) { //If don't found any company that match the requirement just put it back
            companiesPortfolio.push_back(userShareCompany);
        } else //If found then insert the company at that index
            companiesPortfolio.insert(companiesPortfolio.begin() + companyIndex, userShareCompany);
    }
}

/* Function to update player's money*/
void Player::updatePlayerMoney(int money) {
    playerMoney += money;
}

/* Function to update player's number of shares*/
void Player::updatePlayerShares(int shares) {
    playerShares += shares;
}

/* Function to update player's user left*/
void Player::updatePowerUsesLeft() {
    if (powerUsesLeft <= 0) //check if power uses left is smaller than 0 or not
    {
        cout << "You can not use power anymore. You run out of uses" << endl;
    } else
        powerUsesLeft--;
}

/* Function to update the number of companies owned*/
void Player::updateCompaniesOwned() {
    companiesOwned++;
}