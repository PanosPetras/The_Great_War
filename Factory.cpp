#include "Factory.h"

void Factory::Tick() {
	*(TargetStockpile) += materialsProduced + materialsNeeded;
}

Factory::Factory(Stockpile* Target, Market* market, std::string arg, int c = 500) {
	Type = arg;
	cost = c;
	size = 1;
	ChangeOwner(Target);
	TargetMarket = market;
}

Factory::~Factory() {
	if (TargetMarket == nullptr) return;
	(*TargetMarket).Demand += materialsNeeded;
	(*TargetMarket).Supply -= materialsProduced;
}

void Factory::ChangeOwner(Stockpile* NewStockpile) {
	TargetStockpile = NewStockpile;
}

void Factory::confirmMaterials() {
	if (TargetMarket == nullptr) return;
	(*TargetMarket).Demand -= materialsNeeded;
	(*TargetMarket).Supply += materialsProduced;
}

CanningFactory::CanningFactory(Stockpile* Target, Market* market) : Factory(Target, market, "canned food") {
	materialsProduced.Canned_food += 10 * size;

	materialsNeeded.Iron -= 2;
	materialsNeeded.Grain -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

LiquorDistillery::LiquorDistillery(Stockpile* Target, Market* market) : Factory(Target, market, "liquor") {
	materialsProduced.Liquor += 10 * size;

	materialsNeeded.Glass -= 2;
	materialsNeeded.Fruit -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AutomobileFactory::AutomobileFactory(Stockpile* Target, Market* market) : Factory(Target, market, "automobile") {
	materialsProduced.Automobiles += 3 * size;

	materialsNeeded.Lumber -= 4;
	materialsNeeded.Iron -= 4;
	materialsNeeded.Rubber -= 1;
	materialsNeeded.Glass -= 1;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

Shipyard::Shipyard(Stockpile* Target, Market* market) : Factory(Target, market, "merchant ship") {
	daysToProduce = 120;
	days = 0;

	materialsProduced.Merchant_ships += 1 * size;

	materialsNeeded.Lumber -= 4;
	materialsNeeded.Iron -= 4;
	materialsNeeded.Rubber -= 1;
	materialsNeeded.Glass -= 1;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

void Shipyard::Tick() {
	*(TargetStockpile) += materialsNeeded;
	days++;

	if (days == daysToProduce) {
		days = 0;
		*(TargetStockpile) += materialsProduced;
	}
}

LumberMill::LumberMill(Stockpile* Target, Market* market) : Factory(Target, market, "lumber") {
	materialsProduced.Lumber += 15 * size;

	materialsNeeded.Timber -= 10;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

CementFactory::CementFactory(Stockpile* Target, Market* market) : Factory(Target, market, "cement") {
	materialsProduced.Cement += 10 * size;

	materialsNeeded.Coal -= 4;

	confirmMaterials();
}

ClothesFactory::ClothesFactory(Stockpile* Target, Market* market) : Factory(Target, market, "clothes") {
	materialsProduced.Clothes += 10 * size;

	materialsNeeded.Cotton -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

RadioFactory::RadioFactory(Stockpile* Target, Market* market) : Factory(Target, market, "radio") {
	materialsProduced.Radios += 5 * size;

	materialsNeeded.Electric_gear -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

TelephoneFactory::TelephoneFactory(Stockpile* Target, Market* market) : Factory(Target, market, "telephone") {
	materialsProduced.Telephones += 5 * size;

	materialsNeeded.Electric_gear -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

GlassFactory::GlassFactory(Stockpile* Target, Market* market) : Factory(Target, market, "glass") {
	materialsProduced.Glass += 8 * size;

	materialsNeeded.Coal -= 4;

	confirmMaterials();
}

PaperMill::PaperMill(Stockpile* Target, Market* market) : Factory(Target, market, "paper") {
	materialsProduced.Paper += 20 * size;

	materialsNeeded.Lumber -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

FurnitureFactory::FurnitureFactory(Stockpile* Target, Market* market) : Factory(Target, market, "furniture") {
	materialsProduced.Furniture += 7 * size;

	materialsNeeded.Lumber -= 10;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

FuelRefinery::FuelRefinery(Stockpile* Target, Market* market) : Factory(Target, market, "fuel") {
	materialsProduced.Fuel += 8 * size;

	materialsNeeded.Oil -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AmmunitionFactory::AmmunitionFactory(Stockpile* Target, Market* market) : Factory(Target, market, "ammunition") {
	materialsProduced.Ammunition += 5 * size;

	materialsNeeded.Iron -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

PlaneFactory::PlaneFactory(Stockpile* Target, Market* market) : Factory(Target, market, "plane") {
	materialsProduced.Planes += 2 * size;

	materialsNeeded.Lumber -= 4;
	materialsNeeded.Iron -= 1;
	materialsNeeded.Rubber -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ExplosivesFactory::ExplosivesFactory(Stockpile* Target, Market* market) : Factory(Target, market, "explosives") {
	materialsProduced.Explosives += 6 * size;

	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

SmallArmsFactory::SmallArmsFactory(Stockpile* Target, Market* market) : Factory(Target, market, "small arms") {
	materialsProduced.Small_arms += 15 * size;

	materialsNeeded.Iron -= 8;
	materialsNeeded.Lumber -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ArtilleryFactory::ArtilleryFactory(Stockpile* Target, Market* market) : Factory(Target, market, "artillery") {
	materialsProduced.Artillery += 3 * size;

	materialsNeeded.Iron -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

TankFactory::TankFactory(Stockpile* Target, Market* market) : Factory(Target, market, "tank") {
	materialsProduced.Tanks += 1 * size;

	materialsNeeded.Iron -= 4;
	materialsNeeded.Artillery -= 1;
	materialsNeeded.Small_arms -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AirshipFactory::AirshipFactory(Stockpile* Target, Market* market) : Factory(Target, market, "airship") {
	daysToProduce = 200;
	days = 0;

	materialsProduced.Airship += 1 * size;

	materialsNeeded.Iron -= 10;
	materialsNeeded.Electric_gear -= 2;
	materialsNeeded.Cotton -= 12;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

void AirshipFactory::Tick() {
	*(TargetStockpile) += materialsNeeded;
	days++;

	if (days == daysToProduce) {
		days = 0;

		*(TargetStockpile) += materialsProduced;
	}
}

MachinePartFactory::MachinePartFactory(Stockpile* Target, Market* market) : Factory(Target, market, "machine parts") {
	materialsProduced.Machine_parts += 6 * size;

	materialsNeeded.Iron -= 12;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ElectricGearFactory::ElectricGearFactory(Stockpile* Target, Market* market) : Factory(Target, market, "electric gear") {
	materialsProduced.Electric_gear += 8 * size;

	materialsNeeded.Iron -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

SyntheticRubberRefinery::SyntheticRubberRefinery(Stockpile* Target, Market* market) : Factory(Target, market, "rubber") {
	materialsProduced.Rubber += 6 * size;

	materialsNeeded.Coal -= 5;

	confirmMaterials();
}