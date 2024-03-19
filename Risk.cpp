/*******************************************************
    A3: Risky Business Simulator Game
    Phuoc Khoi Nguyen 34166475
 *******************************************************/

#include "Risk.h"
//Constructors
Risk::Risk() {
    // Initialize risk attributes
    riskName = "";
    riskAttributes = "";
    riskMin = 0;
    riskMax = 0;
}

Risk::Risk(string name, string attributes, int min, int max) {
    // Initialize risk attributes with provided values
    riskName = name;
    riskAttributes = attributes;
    riskMin = min;
    riskMax = max;
}

//Accessor Functions
/* Function to return risk's name*/
string Risk::getRiskName() {
    return riskName;
}

/* Function to return risk attributes*/
string Risk::getRiskAttributes() {
    return riskAttributes;
}

/* Function to return risk min*/
int Risk::getRiskMin() {
    return riskMin;
}

/* Function to return risk max*/
int Risk::getRiskMax() {
    return riskMax;
}
//Mutator Functions
/* Function to set risk name*/
void Risk::setRiskName(string name) {
    riskName = name;
}

/* Function to set risk attributes*/
void Risk::setRiskAttributes(string attributes) {
    riskAttributes = attributes;
}

/* Function to set risk min*/
void Risk::setRiskMin(int min) {
    riskMin = min;
}

/* Function to set risk max*/
void Risk::setRiskMax(int max) {
    riskMax = max;
}

