/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#ifndef A3_PHUOCNGUYEN_FINAL_COMPANY_H
#define A3_PHUOCNGUYEN_FINAL_COMPANY_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <sstream>

using namespace std;

class Company {
protected:
    string companyName;
    int companyShares; //set at inherit company (bronze, gold, silver)
    int companyCost; //set at inherit company (bronze, gold, silver)
    int companyLevel; //set at inherit company (bronze, gold, silver)
    int companySharePrice; //set at inherit company (bronze, gold, silver)
    string companyPower;
    string companyOwner;
public:
    //Constructors
    Company();

    Company(string name, int shares, int cost, int level, int sharePrice, string power, string owner);

    //Destructors
    ~Company() = default;

    //Accessor Functions
    string getCompanyName();

    int getCompanyShares();

    int getCompanyCost();

    int getCompanyLevel();

    int getCompanySharePrice();

    string getCompanyPower();

    string getCompanyOwner();

    string getCompanyInfo();

    //Mutator Functions
    void setCompanyName(string name);

    void setCompanyShares(int shares);

    void setCompanyCost(int cost);

    void setCompanyLevel(int level);

    void setCompanyPower(string power);

    void setCompanySharePrice(int price);

    void setCompanyOwner(string owner);

    virtual void setCompanyData(int numberOfPlayers);

    void updateCompanyShares(int shares);
};


#endif //A3_PHUOCNGUYEN_FINAL_COMPANY_H
