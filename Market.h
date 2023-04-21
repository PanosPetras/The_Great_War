#ifndef market_h
#define market_h

#include "Stockpile.h"

class Demand {
    double LinearDemand(double a, double b, int Quantity);

    double HyperbolicDemand(double a, int Quantity);
};

class Market {
public:
    Stockpile Demand;

    Stockpile Supply;

    Stockpile Price;
};

#endif
