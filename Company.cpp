/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#include "Company.h"

//Constructors
Company::Company() {
    // Initialize company attributes
    companyName = "";
    companyShares = 0;
    companyCost = 0;
    companyLevel = 0;
    companyPower = "";
    companySharePrice = 0;
    companyOwner = "Nobody";
}

Company::Company(string name, int shares, int cost, int level, int sharePrice, string power, string owner) {
    // Initialize company attributes with provided values
    companyName = name;
    companyShares = shares;
    companyCost = cost;
    companyLevel = level;
    companyPower = power;
    companySharePrice = sharePrice;
    companyOwner = owner;
}

//Accessor Functions
/* Function to return company's name*/
string Company::getCompanyName() {
    return companyName;
}

/* Function to return company shares*/
int Company::getCompanyShares() {
    return companyShares;
}

/* Function to return cost of the company*/
int Company::getCompanyCost() {
    return companyCost;
}

/* Function to return companyLevel*/
int Company::getCompanyLevel() {
    return companyLevel;
}

/* Function to return companyLevel*/
int Company::getCompanySharePrice() {
    return companySharePrice;
}

/* Function to return the owner of copany*/
string Company::getCompanyOwner() {
    return companyOwner;
}

/* Function to return the power of the company*/
string Company::getCompanyPower() {
    return companyPower;
}

/* Function to return company details*/
string Company::getCompanyInfo() {
    stringstream companyDetails; //Create a formated string
    companyDetails << companyName << "\t" << companyShares << "\t " << companySharePrice << "\t" << companyCost << "\t"
                   << companyOwner << endl;
    return companyDetails.str(); // return the formated string
}

//Mutator Functions
/* Function to set the name for that company*/
void Company::setCompanyName(string name) {
    companyName = name;
}

/* Function to set number of shares for that company*/
void Company::setCompanyShares(int shares) {
    companyShares = shares;
}

/* Function to set the owner for that company*/
void Company::setCompanyOwner(string owner) { //set the owner for the company
    companyOwner = owner;
}

/* Function to set the power for that company*/
void Company::setCompanyPower(string power) { //set the power of that company
    companyPower = power;
}

/* Function to set the cost for that company*/
void Company::setCompanyCost(int cost) {
    companyCost = cost;
}

/* Function to set the level for that company*/
void Company::setCompanyLevel(int level) {
    companyLevel = level;
}

/* Function to set the share price for that company*/
void Company::setCompanySharePrice(int price) {
    //Random a number from 1 to the maximum of company level + 4
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, companyLevel + 4);
    price = dist(gen); //set that random number as price
    companySharePrice = price;
}

/* Function to set company Data this is polymorphism*/
void Company::setCompanyData(int numberOfPlayers) {
    //Default one do nothing
}

/* Update the number of shares*/
void Company::updateCompanyShares(int shares) {
    companyShares += shares;
}