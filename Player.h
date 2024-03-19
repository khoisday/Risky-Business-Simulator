/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#ifndef A3_PHUOCNGUYEN_FINAL_PLAYER_H
#define A3_PHUOCNGUYEN_FINAL_PLAYER_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "Company.h"
#include "Risk.h"

using namespace std;

class Player {
private:
    string playerName;
    int playerMoney;
    int companiesOwned;
    int playerShares;
    int powerUsesLeft;
    vector<Company *> companiesPortfolio; //ADD COMPANY CLASS LATER
public:
    //Constructors
    Player();

    Player(string name, int money, int numberOwn, int shares, int powerLeft);

    //Destructor
    ~Player() = default;

    //Accessor Functions
    string getPlayerName();

    int getPlayerMoney();

    int getCompaniesOwned();

    int getPlayerShares();

    int getPowerUsesLeft();

    vector<Company *> getCompaniesPortfolio();

    string getDetails();

    string getPlayerInfo();

    //Mutator Functions
    void setPlayerName(string name);

    void setPlayerMoney(int money);

    void setCompaniesOwned(int numberOwn);

    void setPlayerShares(int shares);

    void setPowerUsesLeft(int powerLeft);

    void setCompaniesPortfolio(Company *company);

    void updatePlayerMoney(int money);

    void updateCompaniesOwned();

    void updatePlayerShares(int shares);

    void updatePowerUsesLeft();

    void updateCompaniesPortfolio(Company *company, int numShares);
};

#endif //A3_PHUOCNGUYEN_FINAL_PLAYER_H
