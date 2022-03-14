#include "Country.h"

Country::Country(std::string tag, int r, int g, int b, int Res[31]){
	countrytag = tag;
	Country_Population = 0;
	Country_State_Count = 0;
    Policy = {.TaxRate = 50, .Healthcare =  30};
	Color = {.red = r, .green = g, .blue = b};
	Technology = {  .FactoryInput = 1.0f, 
                    .FactoryThroughput = 1.0f, 
                    .FactoryOutput = 1.0f,
                    .MineralOutput = 1.0f,
                    .FarmOutput = 1.0f,
                    .WoodOutput = 1.0f};

    Stock = {       .Coal = Res[0],
                    .Oil = Res[1],
                    .Timber = Res[2],
                    .Rubber = Res[3],
                    .Cotton = Res[4],
                    .Iron = Res[5],
                    .Grain = Res[6],
                    .Fruit = Res[7],
                    .Electric_gear = Res[8],
                    .Machine_parts = Res[9],
                    .Glass = Res[10],
                    .Lumber = Res[11],
                    .Cement = Res[12],
                    .Ammunition = Res[13],
                    .Planes = Res[14],
                    .Explosives = Res[15],
                    .Small_arms = Res[16],
                    .Artillery = Res[17],
                    .Tanks = Res[18],
                    .Canned_food = Res[19],
                    .Furniture = Res[20],
                    .Clothes = Res[21],
                    .Automobiles = Res[22],
                    .Merchant_ships = Res[23],
                    .Radios = Res[24],
                    .Telephones = Res[25],
                    .Fuel = Res[26],
                    .Paper = Res[27],
                    .Liquor = Res[28],
                    .Airship = Res[29],
                    .Money = Res[30]};
}

void Country::AddState(State* state){
	OwnedStates.push_back(state);
    Country_Population = Country_Population + state->State_Population;
	Country_State_Count++;
}

void Country::RemoveState(State* state){
	for (int x = 0; x < OwnedStates.size(); x++) {
		if (OwnedStates[x] == state) {
            Country_Population = Country_Population - state->State_Population;
			OwnedStates.erase(OwnedStates.begin() + x);
			Country_State_Count--;
		}
	}
}

void Country::Tick(){
	for (int x = 0; x < OwnedStates.size(); x++) {
        OwnedStates.at(x)->Tick(Policy.TaxRate, Policy.Healthcare);
	}
    Stock.Money += int(Country_Population * 0.0004 * Policy.TaxRate / 100) - int(Country_Population * 0.0001 * Policy.Healthcare / 100);
}