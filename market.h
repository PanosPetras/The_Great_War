#ifndef market_h
#define market_h

#include "Stockpile.h"

class Demand {
	double LinearDemand(double a, double b, int Quantity);

	double HyperbolicDemand(double a, int Quantity);
};

class Market {
public:
	struct Stockpile D;

	struct Stockpile S;

	void GetSupply(Stockpile Stock);

	void CalculateDemand();
};

#endif