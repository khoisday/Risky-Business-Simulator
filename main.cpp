/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#include "main.h"

int main() {
    gameSetUp();
    //Testing can be done by uncomment these lines ^_^
    //Uncomment the next line to test a player win or not
    //testPlayerWin();
    //Uncomment the next line to test if player is over the limit
    //gameDay=100;
    //Uncomment the next line to test if everyone just leave the game
    //testNobodyLeft();
    //Available options with user choice can be test when playing the games
    //Repeat until when a player met the game end requirement
    while (!checkEnd()) {
        previousPlayerQuit = false; //Check if someone quit or not
        if (checkEnd()) //If check End requirement match the game end
            break;
        userInterface(); //Display what user can interact with
        userChoice(players[playerIndex]); //Function to handle the choice of user
        waitForPlayer();
        clearScreen();
        if (!previousPlayerQuit) //if they quit we won't increase the index
            playerIndex++;
        if (playerIndex ==
            numberOfPlayers) { //If player index exceed the last player index reset the player index, days, and shares price
            playerIndex = 0;
            gameDay++;
            updateDailySharePrice();
        }
    }
}

/*Function to set up the gameplay*/
void gameSetUp() {
    //Read the intro from file and display it
    displayTextFile("RBintro.txt");
    clearScreen();
    //Asking user if they want to load the saved file
    bool validChoice = false;
    while (!validChoice) {
        char load = askForLetter("Do you want to load saved file? (y/n)");
        if ((load == 'Y') || (load == 'N')) { //Limit the choice of load to only Y or N
            validChoice = true; //If load is Y or N set the loop end condition to true;
            if (load == 'N') { //If don't load old data set up the new game
                //Asking user for basic information of the game
                numberOfPlayers = askForNumber("Enter the number of players", 1, 4);
                difficultyLevel = askForNumber("Enter the game mode you want", 1, 3);
                //Set up the basic end game conditions
                if (difficultyLevel == 1)
                    difficultySetUp(500, 3, 12, 40, 6, 4);
                if (difficultyLevel == 2)
                    difficultySetUp(600, 4, 15, 50, 5, 5);
                if (difficultyLevel == 3)
                    difficultySetUp(700, 5, 18, 60, 4, 6);
                clearScreen();
                //Creating players list by asking for player name and set player info based on the game mode
                for (int i = 0; i < numberOfPlayers; i++) {
                    Player *player = new Player();
                    string name = askForString("Input player " + to_string(i + 1) + " name:");
                    player->setPlayerName(name);
                    player->setPlayerMoney(gameModeValue * 5 * 10);
                    player->setPowerUsesLeft(maxPowerUses);
                    players.push_back(player);
                }
                //Read risk and companies data from file
                readRiskData();
                readCompanyData();
                lastCompanyIndex = 'A' + maxCompanies - 1; //Get the first character of the last company's name
                cout << "PRESS ANY KEY TO START THE GAME." << endl;
                waitForPlayer();
                clearScreen();
            } else //If load is Y then load the saved game
            {
                loadSaved();
                lastCompanyIndex = 'A' + maxCompanies - 1; //Get the first character of the last company's name
            }
        }
    }
}

/*Function to read risk data from risk file*/
void readRiskData() {
    ifstream risksData("risksAdvanced.txt"); //Open input file
    if (!risksData.is_open()) //Check if file is open or not
        cout << "Failed to open risks file." << endl;
    else //If file is open read until end of file
        while (!risksData.eof()) {
            //Read data using getline for every Risk stats.
            Risk *risk = new Risk();
            string name;
            string toDo;
            string attributes;
            string minString;
            string maxString;
            getline(risksData, name, ':');
            getline(risksData, toDo, ';');
            getline(risksData, attributes, ';');
            getline(risksData, minString, ';');
            getline(risksData, maxString);
            risk->setRiskName(name);
            risk->setRiskAttributes(attributes);
            risk->setRiskMin(stoi(minString));
            risk->setRiskMax(stoi(maxString));
            risks.push_back(risk); //Push back to the risks vector
        }
    //Close riskData file
    risksData.close();
}

/*Function to read company data from company file */
void readCompanyData() {
    ifstream companiesData("companies.txt"); //Open input file
    if (!companiesData.is_open()) //Check if file open or not
        cout << "Failed to open companies file." << endl;
    else
        //Set up the companies
        for (int i = 0; i < maxCompanies; i++) {
            string level;
            getline(companiesData, level, ';');

            Company *company = nullptr; //Set company as a pointer
            if (level == "1") {
                company = new BronzeCompany(); //Assign company pointer as a new bronze company
                //Set some basic information about those companies based on the number of players, and company level
                company->setCompanyData(numberOfPlayers);
            }

            if (level == "2") {
                company = new SilverCompany(); //Assign company pointer as a new silver  company
                //Set some basic information about those companies based on the number of players, and company level
                company->setCompanyData(numberOfPlayers);
            }

            if (level == "3") {
                company = new GoldCompany(); //Assign company pointer as a new gold company
                //Set some basic information about those companies based on the number of players, and company level
                company->setCompanyData(numberOfPlayers);
            }
            string name;
            //Read from file and set the name, owner, power for each company
            getline(companiesData, name, ';');
            company->setCompanyName(name);
            company->setCompanyOwner("Nobody");
            string power;
            getline(companiesData, power);
            company->setCompanyPower(power);
            //Push that pointer to the companies lisst
            companies.push_back(company);
        }
    //Close companiesData file
    companiesData.close();
}

/* Function to set up the game rules based on difficulty user select
    Parameters: money: the amount of money to win
                winCom: the amount of companies to win
                maxCom: the amount of companies the program have
                days: the maximum play days
                powerUses: the maximum amount of power each player can use
                gameValue: the value depends on game difficulty */
void difficultySetUp(int money, int winCom, int maxCom, int days, int powerUses, int gameValue) {
    winMoney = money;
    winCompanies = winCom;
    maxCompanies = maxCom;
    maxDaysPlay = days;
    maxPowerUses = powerUses;
    gameModeValue = gameValue;
}

/* Function to check the game ended or player win the game or not
    Parameters: player: the player pointers to check that player win the game or not*/
bool checkEnd() {
    stringstream feedback; //Create a string stream for feedback
    bool end = false;
    //Check if they play more than max days available
    if (gameDay > maxDaysPlay) {
        feedback << "This game has ended with no winner because you play more than maximum day"<<endl;
        end = true;
    }
    // Check if nobody left to play the game
    if (players.empty()) {
        feedback << "This game has ended because everyone left the game"<<endl;
        end = true;
    }
    //Check if a player win or not
    for (Player* player:players)
        if ((player->getPlayerMoney() >= winMoney) && (player->getCompaniesOwned() >= winCompanies)) {
            feedback << player->getPlayerName() << " is the winner of this game with " << player->getPlayerMoney()
                     << " money and "
                     << player->getCompaniesOwned() << " companies owned.";
            end = true;
        }
    //If end requirement met give them the feedback and players stats and why this game end
    if (end) {
        clearScreen();
        displayTitle();
        cout << feedback.str() << endl << endl;
        if (!players.empty()) {
            cout << "Stats of all players:" << endl;
            for (int i = 0; i < numberOfPlayers; i++) {
                cout << players[i]->getDetails() << endl;
                cout << "Continue to see other's player stats" << endl;
            }
        }
        waitForPlayer();
        return true;
    }
    return false;
}

/* Function to handle user choice
    Parameters: player: the player pointers to handle data for that player*/
void userChoice(Player *player) {
    bool endTurn = false; //Boolean value to check if the player's turn can end
    bool validChoice = false; //Boolean value to check the choic of that player is valid or not
    char actionChoice = 'Z'; // Initialize actionChoice with a default value

    //Handling with player who have less than 0 money after using risk. If they can not manage to pay the debt.
    //They are forced to quit the game
    while ((player->getPlayerMoney() < 0) && (player->getCompaniesOwned() > 0) &&
           (player->getPowerUsesLeft() > 0)) {
        cout << " You are in debt. Please use power to pay your debt." << endl;
        powerUse(player);
    }
    while ((player->getPlayerMoney() < 0) && (player->getPlayerShares() > 0)) {
        cout << " You are in debt. Please sell shares to pay your debt." << endl;
        sellChoice(player);
    }
    if (player->getPlayerMoney() < 0) {
        quitChoice();
    }
    //Repeat until the end turn condition met
    while (!endTurn) {
        if (!validChoice) { //If player don't select a valid option
            actionChoice = askForLetter("What will you do now?");
        }
        if (actionChoice == 'B') { //Handle with buy option
            endTurn = buyChoice(player);
            validChoice = true;
        } else if (actionChoice == 'S') { //Handle with sell option
            endTurn = sellChoice(player);
            validChoice = true;
        } else if (actionChoice == 'A') { //Handle with acquire option
            endTurn = acquireChoice(player);
            validChoice = true;
        } else if (actionChoice == 'P') { //Handle with power option
            endTurn = powerUse(player);
            validChoice = true;
        } else if (actionChoice == 'R') { //Handle with risk option
            endTurn = riskChoice(player);
            validChoice = true;
        } else if (actionChoice == 'Q') { //Handle with quit game option
            endTurn = quitChoice();
            validChoice = true;
        } else if (actionChoice == 'M') { //Handle with merge option
            endTurn = mergeChoice(player);
            validChoice = true;
        } else if (actionChoice == 'V') { //Handle with save data option
            endTurn = saveData();
            validChoice = true;
        }
    }
}

/* Function to dislay userInterface in their turn*/
void userInterface() {
    //Display the title of game
    displayTitle();
    //Display latest game details
    cout << "    #Companies to win: " << winCompanies << "  Min Money: $" << winMoney << " Day: " << gameDay
         << endl;
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "\t\t    Available Current Company  Current" << endl;
    cout << "\t\t     Shares    Value   Cost     Owner" << endl;
    for (int i = 0; i < maxCompanies; i++) {
        cout << companies[i]->getCompanyInfo();
    }
    //Then get that user info currently
    cout << players[playerIndex]->getPlayerInfo();
    //Display option user could choose
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << " [B]uy [S]ell [A]cquire [P]ower [R]isk [Q]uit [M]erge sa[V]e \n";
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

/* Function to handle saving data for loading the game*/
bool saveData() {
    ofstream savedFile("savedData.txt"); //Open a output file
    if (savedFile.is_open()) {
        cout << " Start file saving." << endl; //Tell player that the game is saving
        //SAVE BASIC INFO GAME
        savedFile << winMoney << " " << winCompanies << " " << maxCompanies << " " << maxDaysPlay << " "
                  << maxPowerUses
                  << " " << gameModeValue << " " << gameDay << " " << numberOfPlayers << " " << difficultyLevel
                  << " "
                  << playerIndex << endl;
        //SAVE COMPANY INFO
        for (Company *company: companies) {
            savedFile << company->getCompanyName() << "," << company->getCompanyShares() << " "
                      << company->getCompanyCost() << " " << company->getCompanyLevel() << " "
                      << company->getCompanySharePrice() << "," << company->getCompanyPower() << ","
                      << company->getCompanyOwner() << endl;
        }
        //SAVE PLAYER ÌNO
        for (Player *player: players) {
            int numberCompanyShares = player->getCompaniesPortfolio().size();
            savedFile << numberCompanyShares << " " << player->getPlayerName() << "," << player->getPlayerMoney()
                      << " "
                      << player->getCompaniesOwned() << " " << player->getPlayerShares() << " "
                      << player->getPowerUsesLeft() << endl;
            for (Company *playerCompany: player->getCompaniesPortfolio()) {
                savedFile << playerCompany->getCompanyName() << "," << playerCompany->getCompanyShares() << " "
                          << playerCompany->getCompanyCost() << " " << playerCompany->getCompanyLevel() << " "
                          << playerCompany->getCompanySharePrice() << "," << playerCompany->getCompanyPower() << ","
                          << playerCompany->getCompanyOwner() << endl;
            }
        }
        //RISK data can be read from file so no need to save
    } else
        cout << " Couldn't open an output file please try again."
             << endl; //Announce player that file output can not open please try again
    cout << " Data is fully saved. You can now turn off the game." << endl; //Announce that file fully saved
    savedFile.close(); //Close output file
    return true;//return true to end player turn
}

/* Function to load saved data*/
void loadSaved() {
    ifstream savedData("savedData.txt"); //Open input file
    if (savedData.is_open()) { //Check if file is open or not if open load game data
        cout << "Start loading data." << endl; //Tell player that game data is being loaded
        string line;
        //Load basic info of the game
        string playerIndexString = "";
        savedData >> winMoney >> winCompanies >> maxCompanies >> maxDaysPlay >> maxPowerUses >> gameModeValue
                  >> gameDay
                  >> numberOfPlayers >> difficultyLevel;
        getline(savedData, playerIndexString);
        playerIndex = stoi(playerIndexString);
        //Load companies details by creating
        string companyName = "";
        int companyShares = 0;
        int companyCost = 0;
        int companyLevel = 0;
        int companySharePrice = 0;
        string companyPower = "";
        string companyOwner = "";
        string companySharePriceString = "";
        string powerUsesLeftString = "";
        for (int i = 0; i < maxCompanies; i++) {
            getline(savedData, companyName, ',');
            savedData >> companyShares >> companyCost >> companyLevel;
            getline(savedData, companySharePriceString, ',');
            companySharePrice = stoi(companySharePriceString);
            getline(savedData, companyPower, ',');
            getline(savedData, companyOwner);
            //Create a new company using the overload function
            Company *company = new Company(companyName, companyShares, companyCost, companyLevel, companySharePrice,
                                           companyPower, companyOwner);
            companies.push_back(company); //Push that new create company to the vector
        }
        //Load player details
        for (int i = 0; i < numberOfPlayers; i++) {
            string numberCompanyString;
            int numberCompany = 0;
            string playerName = "";
            int playerMoney = 0;
            int playerCompaniesOwned = 0;
            int playerShares = 0;
            int playerPowerUsesLeft = 0;
            getline(savedData, numberCompanyString, ' ');
            numberCompany = stoi(numberCompanyString);
            getline(savedData, playerName, ',');
            savedData >> playerMoney >> playerCompaniesOwned >> playerShares;
            getline(savedData, powerUsesLeftString);
            playerPowerUsesLeft = stoi(powerUsesLeftString);
            Player *player = new Player(playerName, playerMoney, playerCompaniesOwned, playerShares,
                                        playerPowerUsesLeft);
            //Load player companies portfolio
            for (int companyIndex = 0; companyIndex < numberCompany; companyIndex++) {
                string companySharePriceString;
                getline(savedData, companyName, ',');
                savedData >> companyShares >> companyCost >> companyLevel;
                getline(savedData, companySharePriceString, ',');
                companySharePrice = stoi(companySharePriceString);
                getline(savedData, companyPower, ',');
                getline(savedData, companyOwner);
                //Create a new company in player's companies portfolio using the overload function
                Company *playerCompany = new Company(companyName, companyShares, companyCost, companyLevel,
                                                     companySharePrice,
                                                     companyPower, companyOwner);
                player->setCompaniesPortfolio(
                        playerCompany); //set the companies portfolio equal as the vector playerCompaniesList
            }
            players.push_back(player); //push back the player to the players list
        }
        //Load risk data
        readRiskData();
        cout << "Data is fully loaded. You can play the game now."
             << endl; //Announce that data is loaded so player can play the game
        waitForPlayer();
        clearScreen();
        savedData.close();
    }
}

/* Function to handle when user select buy
    Parameters: player: the player pointers to handle buy shares request for that player*/
bool buyChoice(Player *player) {
    bool endTurn = false; //Boolean to check for end that player turn
    int minMoneyNeed = 10; //10 is the highest price for a share
    //Finding the cheapest price for a share
    for (int i = 0; i < maxCompanies; i++) {
        if (minMoneyNeed > companies[i]->getCompanySharePrice())
            minMoneyNeed = companies[i]->getCompanySharePrice();
    } //If you can not afford the cheapest one then print they don't have money to buy any shares
    if (player->getPlayerMoney() <= minMoneyNeed) {
        cout << " You can not sell any shares. Please sell or use power" << endl;
        return true;
    }
    string question =
            "Buy shares in which company [A-" + string(1, lastCompanyIndex) +
            "]"; //Ask which company shares they want
    int companyIndex = 30; //Big number for the loop to repeat
    //Repeat until find a data in the shares pool
    while ((companyIndex > maxCompanies) || (companyIndex < 0)) {
        char companyChoice = askForLetter(question);
        companyIndex = companyChoice - 'A';
        if ((companyIndex > maxCompanies) || (companyIndex < 0))
            cout << " Please select another character. No data about that company." << endl;
    }
    //Check if there's any shares left at that company or not
    int sharesAvailable = companies[companyIndex]->getCompanyShares();
    if (sharesAvailable > 0) {
        int numShares = askForNumber(" How many shares to buy", 1, sharesAvailable);
        buyShares(player, companies[companyIndex], numShares); //Handling with buy shares
    } else
        cout << " There's no more available shares at that company." << endl;

    //Ask player do they want to buy more shares
    bool validChoice = false;
    while (!validChoice) {
        char buyMore = askForLetter("Do you want to buy more shares? (y/n)");
        if ((buyMore == 'Y') || (buyMore == 'N')) {
            validChoice = true;
            if (buyMore == 'N')
                endTurn = true;
        }
    }
    return endTurn;
}

/* Function to handle user and company data when buy shares happen
    Parameters: player: the player pointers to handle data for that player
                company: the company that player want to buy
                numShares: how many shares they want to buy*/
void buyShares(Player *player, Company *company, int numShares) {
    int playerMoney = player->getPlayerMoney();
    int shares = company->getCompanyShares();
    int sharePrice = company->getCompanySharePrice();
    //Check if they got enough money and want less than the available shares
    //If Yes then start the selling action
    if ((numShares <= shares) && (playerMoney >= (numShares * sharePrice))) {
        player->updatePlayerMoney(-(numShares * sharePrice)); //Minus player money
        player->updatePlayerShares(numShares); //update player number of shares
        company->updateCompanyShares(-numShares); //update that company shares in the pool
        player->updateCompaniesPortfolio(company, numShares); //update the companies portfolio list
    } else { //If they don't met the requirement ask them to check their balance and print their money left
        cout << "Unable to buy that amount of shares. Please check your balance and share available." << endl;
        cout << " You have: " << player->getPlayerMoney() << " $ left" << endl;
    }
}

/* Function to handle when user select sell
    Parameters: player: the player pointers to handle sell shares request for that player*/
bool sellChoice(Player *player) {
    bool endTurn = false;
    //Check if player got any shares in their own shares pool
    if (player->getPlayerShares() <= 0) {
        cout << "You don't have any shares to sell. Please buy more" << endl;
        return endTurn = true;
    }
    //Ask user  which company they want to sell shares in
    string question = "Sell shares in which company [A-" + string(1, lastCompanyIndex) + "]";
    bool companyOwned = false; //boolean to check if we found any company match that user request
    Company *company = nullptr; //Create a instance for Company pointer right now temporarily set it as null pointer
    while (!companyOwned) { //Loop until find one company
        char companyChoice = askForLetter(question); //Ask user what company they want
        //Loop in the player companies list to check if they own shares in that company
        for (int i = 0; i < player->getCompaniesPortfolio().size(); i++) {
            //Check for the letter inside the bracket match with the choice or not and that company have shares or not
            if ((companyChoice == player->getCompaniesPortfolio()[i]->getCompanyName()[1]) &&
                (player->getCompaniesPortfolio()[i]->getCompanyShares() > 0)) {
                company = player->getCompaniesPortfolio()[i];
                companyOwned = true;
                break;
            }
        }
        //If company not found annouce this
        if (!companyOwned)
            cout << "You do not have any share at that company. Please reselect." << endl;
    }
    //Ask user how many sahres they want
    int numShares = askForNumber(" How many shares to sell", 1, company->getCompanyShares());
    sellShares(player, company, numShares); //Function to handle with sell sahres
    //Check if they want to sell more shares or not
    bool validChoice = false;
    while (!validChoice) {
        char sellMore = askForLetter("Do you want to sell more shares? (y/n)");
        if ((sellMore == 'Y') || (sellMore == 'N')) {
            validChoice = true;
            if (sellMore == 'N')
                endTurn = true;
        }
    }
    return endTurn;
}

/* Function to handle user and company data when buy shares happen
    Parameters: player: the player pointers to handle data for that player
                company: the company that player want to sell
                numShares: how many shares they want to sell*/
void sellShares(Player *player, Company *company, int numShares) {
    int playerShares = company->getCompanyShares(); //Shares that player have
    int sharePrice = companies[company->getCompanyName()[1] - 'A']->getCompanySharePrice(); //Current price of share
    if ((numShares <= playerShares)) { //If they want to sell less or equal to what they have start to update data
        player->updatePlayerMoney(numShares * sharePrice); //increase their money
        player->updatePlayerShares(-numShares); //minus their shares
        //Get the index of company in the companies list by using minus in ASCII. Get the first character of the company name and minus A we can get the index
        company = companies[company->getCompanyName()[1] - 'A']; //Using that index to set the company pointer
        company->updateCompanyShares(numShares); //Update company shares
        player->updateCompaniesPortfolio(company, -numShares); //Update companies portfolio of player
    } else {
        //Announce player if they don't have enough shares
        cout << "Unable to sell that amount of shares. Please check your share amount." << endl;
    }
}

/* Function to handle when user select acquire
    Parameters: player: the player pointers to handle acquire company request for that player*/
bool acquireChoice(Player *player) {
    bool endTurn = false; //boolean value to check for end player's turn
    //Check if player have at least some shares to acquire
    if (player->getPlayerShares() == 0) {
        cout << "You don't own any shares. Please buy shares then acquire new company" << endl;
        return endTurn = true;
    }
    bool canAcquire = false;
    for (Company *userShareCompany: player->getCompaniesPortfolio()) {
        if (userShareCompany->getCompanyShares() ==
            companies[userShareCompany->getCompanyName()[1] - 'A']->getCompanyCost()) {
            canAcquire = true;
            break;
        }
    }
    if (!canAcquire) {
        cout << "You don't own enough shares to acquire one company" << endl;
        return endTurn = true;
    }
    string question = "You want to acquire which company [A-" + string(1, lastCompanyIndex) + "]";
    bool companyOwned = false; //boolean value to check if is it company in the players's companies portfolio
    int companyIndex = -1;
    Company *company = nullptr;
    while (!companyOwned) { //Loop until found a company
        char companyChoice = askForLetter(question);     //Ask the user what company they want to choose
        for (int i = 0; i < player->getCompaniesPortfolio().size(); i++) {
            if (companyChoice ==
                player->getCompaniesPortfolio()[i]->getCompanyName()[1]) { //Check until found a company name's first letter match with user choice
                company = player->getCompaniesPortfolio()[i]; //Set the company pointer to a company in player's companies portfolio
                companyOwned = true; //Assign that company has been found
                companyIndex = companyChoice - 'A'; //Set that companyIndex as i (index of the found company)
                break;
            }
        }
        if (!companyOwned) //If no company found ask user to reselect
            cout << "You do not have any share at that company. Please reselect." << endl;
        //If they don't have enough shares ask them to merge with others or buy new shares.
        if (company->getCompanyShares() < companies[companyChoice - 'A']->getCompanyCost()) {
            cout << "You do not own every shares in that company. Please merge with others or buy new shares."
                 << endl;
        } else { //If they can manage to acquire the company
            player->updatePlayerShares(-company->getCompanyShares()); //Minus all the shares that company have.
            player->updateCompaniesOwned(); //Add number of companies own by user by 1
            company->setCompanyPower(companies[companyIndex]->getCompanyPower()); //Set the power for that company
            companies[companyIndex]->setCompanyOwner(
                    player->getPlayerName()); //Set the owner for that company in the companies list to display for user
            company->setCompanyShares(0); //Erase all the shares in that company
        }
    }
    //Ask user if they want to acquire more company
    bool validChoice = false;
    while (!validChoice) {
        char acquireMore = askForLetter("Do you want to acquire more company? (y/n)");
        if ((acquireMore == 'Y') || (acquireMore == 'N')) {
            validChoice = true;
            if (acquireMore == 'N')
                endTurn = true;
        }
    }
    return endTurn;
}

/* Function to handle when user select power
Parameters: player: the player pointers to handle power request for that player*/
bool powerUse(Player *player) {
    bool endTurn = false; //Boolean to check end player's turn
    if (player->getPowerUsesLeft() > 0) { //Check if they have any power use left
        //Check if they have own any company cause they can only use power if they have at least 1 company
        if (player->getCompaniesOwned() == 0) {
            cout << "You don't own any company to use their power." << endl;
            endTurn = true;
        } else { //if they own any ask user what company power they want to choose
            string question = "Use the Corporate Power of which company [A-" + string(1, lastCompanyIndex) + "]";
            bool companyOwned = false; //Boolean value to check if company is found or not
            Company *company = nullptr;
            while (!companyOwned) { //Repeat until found a company
                char companyChoice = askForLetter(question);
                for (int i = 0; i < player->getCompaniesPortfolio().size(); i++) {
                    if (companyChoice ==
                        player->getCompaniesPortfolio()[i]->getCompanyName()[1]) { //Check if any company match
                        company = player->getCompaniesPortfolio()[i];
                        companyOwned = true;
                        break;
                    }
                }
                string companyPower = company->getCompanyPower(); //get the companyPower
                srand(time(NULL));
                int multiplier = rand() % gameModeValue + 2;
                //Check if condition for companyPower and update player data
                if (companyPower == "+ money") {
                    //Update money then reduce the power use left of player
                    cout << "Money Bonus: " << 10 * multiplier * gameModeValue << endl;
                    player->updatePlayerMoney(10 * multiplier * gameModeValue);
                    player->updatePowerUsesLeft();
                    endTurn = true;
                }
                if (companyPower == "+ shares") {
                    //Check for available shares at the company if it is smaller than bonus random shares
                    //Then the bonus then become the number of avalable shares
                    //Then update the details such as shares in the data pool and shares in player's list
                    int availableShares = companies[companyChoice - 'A']->getCompanyShares();
                    int sharesBonus = multiplier * gameModeValue;
                    if (availableShares <= sharesBonus) {
                        sharesBonus = availableShares;
                    }
                    cout << "Shares Bonus: " << sharesBonus << endl;
                    player->updateCompaniesPortfolio(companies[companyChoice - 'A'], sharesBonus);
                    player->updatePlayerShares(sharesBonus);
                    companies[companyChoice - 'A']->updateCompanyShares(-sharesBonus);
                    player->updatePowerUsesLeft();
                    endTurn = true;
                }
                if (companyPower == "+ assets") {
                    //The combination of money and shares
                    int availableShares = companies[companyChoice - 'A']->getCompanyShares();
                    int sharesBonus = multiplier * gameModeValue;
                    if (availableShares <= sharesBonus) {
                        sharesBonus = availableShares;
                    }
                    cout << "Money Bonus: " << 10 * multiplier * gameModeValue << endl;
                    cout << "Shares Bonus: " << sharesBonus << endl;
                    player->updatePlayerMoney(10 * multiplier * gameModeValue);
                    player->updateCompaniesPortfolio(companies[companyChoice - 'A'], sharesBonus);
                    player->updatePlayerShares(sharesBonus);
                    companies[companyChoice - 'A']->updateCompanyShares(-sharesBonus);
                    player->updatePowerUsesLeft();
                    endTurn = true;
                }
            }
        }
    } else { //When player have no use left annouce them
        cout << "You don't have any uses left." << endl;
        endTurn = true;
        return endTurn;
    }
    return endTurn;
}

/* Function to handle when user select power
Parameters: player: the player pointers to handle risk request for that player*/
bool riskChoice(Player *player) {
    //using random device based on mt19937 pseudo code in order to shuffle the vector
    random_device rd;
    mt19937 seed(rd());
    shuffle(risks.begin(), risks.end(), seed); //shuffle from the begin to the end of risks vector
    //Random from 0 to 19 to select a risk from risks list
    uniform_int_distribution<int> dist(0, 19);
    int randomRiskChoice = dist(seed);
    Risk *risk = risks[randomRiskChoice];
    //Get info about risks
    int min = risk->getRiskMin();
    int max = risk->getRiskMax();
    string riskName = risk->getRiskName();
    string attributes = risk->getRiskAttributes();
    cout << " Global event: " << riskName << endl;
    //Money attributes
    if (attributes == "money") {
        moneyRisk(player, min, max);
    }
    //Shares attributes
    if (attributes == "shares") {
        sharesRisk(player, min, max);
    }
    //Combination of money and shares
    if (attributes == "assets") {
        moneyRisk(player, min, max);
        sharesRisk(player, min, max);
    }
    return true;
}

/* Function to handle risk with money
Parameters: player: the player pointers to handle money risk for that player
            min: the min value of risk
            max: the max value of risk*/
void moneyRisk(Player *player, int min, int max) {
    srand(time(NULL));
    cout << " Risking your money." << endl; //Announce that player is risking their shares
    if (min < 0) { //Check min of risk if it less than 0 then do the risk for everybody
        int randomMultipler = rand() % (max + 1);
        //Add or minus money from everybody using loop
        for (Player *p: players) {
            max = abs(max);
            if (max > 0) { //If max >0 add money for everyone
                cout << " Money bonus for everybody: " << randomMultipler * gameModeValue << endl;
                p->updatePlayerMoney(randomMultipler * gameModeValue);
            }
            if (max < 0) { //If max <0 minus money for everyone
                cout << " Money deduction for every: " << -randomMultipler * gameModeValue << endl;
                p->updatePlayerMoney(-randomMultipler * gameModeValue);
            }
        }
    } else { //If min>=0
        if (max > 0) { //if max>0 then increase that player money by using a random number from min to max value
            int randomMultipler = rand() % (max - min + 1) + min;
            cout << " Money Bonus: " << gameModeValue * randomMultipler << endl;
            player->updatePlayerMoney(gameModeValue * randomMultipler);
        } else { //if max<=0 then decrease that player money by using a random number from min to absolute max value
            max = abs(max); //set max to the absolute of max
            int randomMultipler = rand() % (max - min + 1) + min;
            cout << " Money Penalty: " << -(gameModeValue * randomMultipler) << endl;
            player->updatePlayerMoney(-(gameModeValue * randomMultipler));
        }
    }
}

/* Function to handle when risk with shares
Parameters: player: the player pointers to handle shares risk for that player
            min: the min value of risk
            max: the max value of risk*/
void sharesRisk(Player *player, int min, int max) {
    srand(time(NULL));
    cout << " Risking your shares." << endl;
    //If player have no shares, risk function do nothing to them
    if (player->getPlayerShares() <= 0) {
        cout << "Player don't have any shares. End risk." << endl;
        return;
    }
    int sharesUpdate = 0;
    string question = "Risk shares in which company [A-" + string(1, lastCompanyIndex) + "]";
    bool companyOwned = false; //Boolean value to check if we found that company
    Company *company = nullptr;
    while (!companyOwned) {
        char companyChoice = askForLetter(question);
        //Check if any first letter of company name
        for (int i = 0; i < player->getCompaniesPortfolio().size(); i++) {
            if (companyChoice == player->getCompaniesPortfolio()[i]->getCompanyName()[1]) {
                company = player->getCompaniesPortfolio()[i];
                companyOwned = true;
                break;
            }
        }
        //If not found tell player
        if (!companyOwned)
            cout << " You do not have any share at that company. Please reselect." << endl;
    }
    //Get the available share in the shares data pool
    int sharesAvailable = companies[company->getCompanyName()[1] - 'A']->getCompanyShares();
    //Get the avalailble shares of that player
    int playerSharesAvailable = company->getCompanyShares();
    //If max >0 take shares from the data poool
    if (max > 0) {
        //Get the update shares by using random
        sharesUpdate = rand() % (max - min + 1) + min;
        //If shares available is less than number of shares, update sharesUpdate equal the amount of shares available
        if (sharesAvailable < sharesUpdate) {
            sharesUpdate = sharesAvailable;
        }
        cout << " Company Share Bonus: " << sharesUpdate << endl;

    } else {
        //Update shares by using random from min to the abslute of max
        sharesUpdate = -(rand() % (abs(max) - min + 1) + min);
        if (playerSharesAvailable <
            sharesUpdate) //if the player shares available is less than what the number of update shares
            //Set updat shares to the available one of that player
            sharesUpdate = playerSharesAvailable;
        cout << " Company Share Penalty: " << sharesUpdate << endl;
    }
    company = companies[company->getCompanyName()[1] -
                        'A']; //Set company pointer to that company in the companies data pool
    company->updateCompanyShares(-sharesUpdate); //UPDATE SHARES POOL if max <0 add to pool max >0 take from pool
    player->updateCompaniesPortfolio(company, sharesUpdate); //Update the companies portfolio of users
}

/* Function to handle when risk with shares*/
bool quitChoice() {
    numberOfPlayers--; //Reduce the number of player
    Player *player = players[playerIndex]; //Get the player who is quitting
    //Loop through the companies in the player's company portfolio
    for (Company *company: player->getCompaniesPortfolio()) {
        int companyIndex = (company->getCompanyName()[1] - 'A'); //Get the index of company in companies vector
        companies[companyIndex]->updateCompanyShares(
                company->getCompanyShares()); //Give back all the shares that user have
        //Exception when player own that company they have to return all the shares to buy that company
        if (companies[companyIndex]->getCompanyOwner() ==
            player->getPlayerName()) { //If that player own the company set the owner back to nobody
            companies[companyIndex]->setCompanyOwner("Nobody");
            //Return the orignal shares
            int sharesReturn = companies[companyIndex]->getCompanyCost();
            companies[companyIndex]->updateCompanyShares(sharesReturn);
        }
    }
    players.erase(players.begin() + playerIndex); //erases that player from the players vector
    previousPlayerQuit = true;
    return true;
}

/* Function to handle merging shares with another player */
bool mergeChoice(Player *player1) {
    bool endTurn = false; //Boolean to return player can endTurn or not

    // If the player has no shares, end their turn
    if (player1->getPlayerShares() <= 0) {
        cout << "You don't have any shares to merge. Please buy more" << endl;
        return endTurn = true;
    }
    string question = "Merge shares in which company [A-" + string(1, lastCompanyIndex) + "]";
    bool companyOwned = false;
    Company *company = nullptr;
    char companyChoice = 'Z';
    while (!companyOwned) { //Loop until found a company
        companyChoice = askForLetter(question);     //Ask user what company they want to buy
        for (int i = 0; i < player1->getCompaniesPortfolio().size(); i++) { //Loop through companies portfolio
            if (companyChoice ==
                player1->getCompaniesPortfolio()[i]->getCompanyName()[1]) { //Check company that match companyChoice
                company = player1->getCompaniesPortfolio()[i]; //Set company as the company match with companyChoice
                companyOwned = true; //set company owned to know that we've found that company
                break;
            }
        } //If company not found then ask user to reselect
        if (!companyOwned)
            cout << "You do not have any share at that company. Please reselect." << endl;
    }
    vector<int> othersIndex; //Create a vector to get others player who own shares at that company too
    othersIndex.clear(); //Reset that vector to an empty one
    cout << "Finding others that own share at that company..." << endl;
    //Loop through everybody to check if they own that company
    for (int i = 0; i < numberOfPlayers; i++) {
        for (Company *playerCompany: players[i]->getCompaniesPortfolio()) { //Loop through each players portfolio list
            if ((company->getCompanyName() == playerCompany->getCompanyName()) &&
                (company->getCompanyShares() > 0)) { //Check if they own shares at that company
                if (players[i] != player1) //If that player is different from the one who want to merge
                    othersIndex.push_back(i); //Add that to the othersIndex vector
            }
        }
    }
    if (othersIndex.empty()) { //If that vector is empty say
        cout << "You are the only one who own shares in that company." << endl;
        return endTurn = true;
    } else { //If others index is not empty start the merge shares
        cout << "Player index range: (0-" << numberOfPlayers - 1 << ")" << endl;
        //Display the index of every others player who have shares at that company
        for (int i = 0; i < othersIndex.size(); i++) {
            cout << "Player " << othersIndex[i] << " own shares at that company. " << endl;
        }
        //ask which player they want to merge
        bool playerOwned = false;
        char playerIndexChoice;
        while (!playerOwned) {
            playerIndexChoice = askForNumber("Which player do you want to merge shares with", 0,
                                             numberOfPlayers - 1);
            for (int index = 0; index < othersIndex.size(); index++) {
                if (playerIndexChoice == othersIndex[index])
                    playerOwned = true;
            }
        }
        //Loop through every single player in the
        for (Company *myCompany: players[playerIndexChoice]->getCompaniesPortfolio()) {
            //Check for company that match the company choice of usser
            if (myCompany->getCompanyName()[1] == companyChoice) {
                //Get the money of player 1, how much shares they merge, the current sharePrice, the total cost of merge function
                int player1Money = player1->getPlayerMoney();
                int sharesMerge = myCompany->getCompanyShares();
                int sharesPrice = companies[companyChoice - 'A']->getCompanySharePrice();
                int mergePrice = sharesMerge * sharesPrice;
                if (player1Money >= mergePrice) { //Check if player 1 can afford the merge
                    //If yes update player 1 and player 2, money shares and company portfolio
                    Player *player2 = players[playerIndexChoice];
                    cout << "Start to merge shares" << endl;
                    player1->updateCompaniesPortfolio(company, sharesMerge);
                    player2->updateCompaniesPortfolio(myCompany, -sharesMerge);
                    player1->updatePlayerMoney(-mergePrice);
                    player2->updatePlayerMoney(mergePrice);
                    player1->updatePlayerShares(+sharesMerge);
                    player2->updatePlayerShares(-sharesMerge);
                } else
                    cout << "You aren't able to merge with this player. Please check again" << endl;
            }
        }
    }
    return true;
}

/* Function to set the share price for companies reset it daily*/
void updateDailySharePrice() {
    //Using srand time null random it from 1 to companyLevel + 4;
    for (int index = 0; index < maxCompanies; index++) {
        srand(time(NULL));
        int price = rand() % (companies[index]->getCompanyLevel() + 4) + 1;
        companies[index]->setCompanySharePrice(price);
    }
}

/* Function to randomly select a player in the players list and make them the winner*/
void testPlayerWin() {
    //Random a player from 0 -> 3 in the playrs list and set them as a winner
    random_device rd;
    mt19937 index(rd());
    uniform_int_distribution<> random(0, 3);
    int winIndex = random(index);
    //Upgrade that player amount of money by 10000 and 10 companies owned to 100% win the game :D He's so rich
    for (int i = 0; i < 10; i++) {
        players[winIndex]->updatePlayerMoney(1000);
        players[winIndex]->updateCompaniesOwned();
    }
}

/* Function to delete everybody from the vector*/
void testNobodyLeft(){
    while (numberOfPlayers>0){
        players.erase(players.begin());
        numberOfPlayers--;
    }
    players.clear();
}



/*******************************************************
  Project: WORKHORSE Functions
   Author: Cheryl Howard
  Purpose: These are permitted reusable functions.
  You are free to include these in your assignments as
  long as you include both comments.
 *******************************************************/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// you MUST include: waitForPlayer() and clearScreen()
void waitForPlayer() {
    // adjust new line and indents as required
    // Windows
    system("pause");
    // Mac & Linux
    //system("read -n 1 -s -r -p \"Press any key to continue\"");
}

void clearScreen() {
    // Windows
    system("cls");
    // Mac & Linux
    //system("clear");
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Using the following functions is optional
void displayTitle() {
    // use this if NOT using the title as a constant
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "      Risky Business :: Share Market Simulation\n";
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void displayTextFile(string myFile) {
    // local vars
    ifstream blurbData(myFile);
    string line = "";

    // read in file data
    if (blurbData.is_open()) {
        while (getline(blurbData, line)) {
            cout << line << "\n";
        }
    }
    blurbData.close();

    waitForPlayer();
}

string askForString(string question) {
    // gets a line response (spaces included)
    int timeLoop = 0;
    string userInput = "";
    while (userInput.empty()) {
        if (timeLoop == 0) //Make a loop counter to stop the get line glitch to pop 2 question
            cout << question << endl;
        getline(cin, userInput);
        timeLoop++;
    }
    return userInput;
}

//Sorry for fixing this one, I found there's a bug when input a string such as 1b, in order to handle string, i fix it a bit
int askForNumber(string question, int min, int max) {
    // ask for a numeric response from the user
    int number;
    string input = "Fix for handeling if user input string";
    bool isDigit = false;
    bool isValid = false;
    while ((!isValid) || !isDigit) {
        isValid = true;
        isDigit = true;
        cout << question << " (" << min << "-" << max << "): ";
        cin >> input;
        for (int i = 0; i < input.size(); i++) {
            if (!isdigit(input[i])) {
                isDigit = false;
                break;
            }
        }
        if (isDigit) {
            number = stoi(input);
            if ((number < min || number > max)) {
                isValid = false;
            }
        } else {
            cout << "Please input an integer" << endl;
        }
    }
    return number;
}

char askForLetter(string question) {
    string input;
    bool validCheck;
    do {
        cout << "\n " << question;
        cin >> input;     // get a line of input
        validCheck = true;              // assume it's valid
        if (input.size() == 1) {
            if (!isalpha(input[0]))       // is it an alphabetical character?
            {
                validCheck = false;     // if not, mark it as invalid
                cout << "Invalid input. Please only input alphabetical character." << endl;
            }
        }
        if (input.size() > 1) {
            cout << "Invalid input. Please only input 1 character" << endl;
            validCheck = false;
        }
    } while (!validCheck);              // keep going until we get input that's valid
    return toupper(input[0]);
}

/**************************************************
  END of WORKHORSE functions
 **************************************************/