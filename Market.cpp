#include "market.h"

double Demand::LinearDemand(double a, double b, int Quantity){
	return (a - Quantity) / b;
}

double Demand::HyperbolicDemand(double a, int Quantity){
	if (Quantity == 0) {
		return 0.0;
	}
	return a / Quantity;
}

void Market::GetSupply(Stockpile Stock){
	S = {	Stock.Coal,
			Stock.Oil,
			Stock.Timber,
			Stock.Rubber,
			Stock.Cotton,
			Stock.Iron,
			Stock.Grain,
			Stock.Fruit,
			Stock.Electric_gear,
			Stock.Machine_parts,
			Stock.Glass,
			Stock.Lumber,
			Stock.Cement,
			Stock.Ammunition,
			Stock.Planes,
			Stock.Explosives,
			Stock.Small_arms,
			Stock.Artillery,
			Stock.Tanks,
			Stock.Canned_food,
			Stock.Furniture,
			Stock.Clothes,
			Stock.Automobiles,
			Stock.Merchant_ships,
			Stock.Radios,
			Stock.Telephones,
			Stock.Fuel,
			Stock.Paper,
			Stock.Liquor,
			Stock.Airship,
			0};
}

void Market::CalculateDemand()
{
}
