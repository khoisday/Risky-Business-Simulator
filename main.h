/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#ifndef A3_PHUOCNGUYEN_FINAL_MAIN_H
#define A3_PHUOCNGUYEN_FINAL_MAIN_H

#include "Player.h"
#include "BronzeCompany.h"
#include "SilverCompany.h"
#include "GoldCompany.h"
#include <fstream>
#include <algorithm>

using namespace std;

int winMoney;
int winCompanies;
int maxCompanies;
int maxDaysPlay;
int maxPowerUses;
int gameModeValue;
int gameDay = 1;
int numberOfPlayers = -1;
int difficultyLevel = 1;
int playerIndex=0;
char lastCompanyIndex = 'A' + maxCompanies - 1;
bool previousPlayerQuit= false;

vector<Player *> players;
vector<Company *> companies;
vector<Risk *> risks;

void gameSetUp();

void readRiskData();

void readCompanyData();

void difficultySetUp(int money, int winCom, int maxCom, int days, int powerUses, int gameValue);

bool checkEnd();

void userChoice(Player *player);

void userInterface();

bool saveData();

void loadSaved();

bool buyChoice(Player *player);

void buyShares(Player *player, Company *company, int numShares);

bool sellChoice(Player *player);

void sellShares(Player *player, Company *company, int numShares);

bool acquireChoice(Player *player);

bool powerUse(Player *player);

bool riskChoice(Player *player);

void moneyRisk(Player* player,int min, int max);

void sharesRisk(Player* player,int min,int max);

bool quitChoice();

bool mergeChoice(Player* player1);

void testPlayerWin();

void testNobodyLeft();

void displayTitle();

void waitForPlayer();

void clearScreen();

void updateDailySharePrice();

void displayTextFile(string myFile);

string askForString(string question);

int askForNumber(string question, int min, int max);

char askForLetter(string question);


#endif //A3_PHUOCNGUYEN_FINAL_MAIN_H
