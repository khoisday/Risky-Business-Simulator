/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#ifndef A3_PHUOCNGUYEN_FINAL_RISK_H
#define A3_PHUOCNGUYEN_FINAL_RISK_H

#include <iostream>
#include <string>
#include <sstream>


using namespace std;

class Risk {
private:
    string riskName;
    string riskAttributes;
    int riskMin;
    int riskMax;
public:
    //Constructors
    Risk();

    Risk(string name, string attributes, int min, int max);

    //Destructor
    ~Risk() = default;

    //Accessor Functions
    string getRiskName();

    string getRiskAttributes();

    int getRiskMin();

    int getRiskMax();

    //Mutator Functions
    void setRiskName(string name);

    void setRiskAttributes(string attributes);

    void setRiskMin(int min);

    void setRiskMax(int max);

};


#endif //A3_PHUOCNGUYEN_FINAL_RISK_H
