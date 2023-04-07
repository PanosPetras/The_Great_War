#include "Market.h"

double Demand::LinearDemand(double a, double b, int Quantity){
	return (a - Quantity) / b;
}

double Demand::HyperbolicDemand(double a, int Quantity){
	if (Quantity == 0) {
		return 0.0;
	}
	return a / Quantity;
}
