/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#ifndef A3_PHUOCNGUYEN_FINAL_GOLDCOMPANY_H
#define A3_PHUOCNGUYEN_FINAL_GOLDCOMPANY_H

#include "Company.h"

class GoldCompany : public Company {
public:
    GoldCompany();

    GoldCompany(string name, int shares, int cost, int level, int sharePrice, string power, string owner);

    void setCompanyData(int numberOfPlayers) override;
};


#endif //A3_PHUOCNGUYEN_FINAL_GOLDCOMPANY_H
