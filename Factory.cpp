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

CanningFactory::CanningFactory(Stockpile* Target, Market* market) : Factory(Target, market, "canned food", 8000) {
	materialsProduced.Canned_food += 10 * size;

	materialsNeeded.Iron -= 2;
	materialsNeeded.Grain -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

LiquorDistillery::LiquorDistillery(Stockpile* Target, Market* market) : Factory(Target, market, "liquor", 11000) {
	materialsProduced.Liquor += 10 * size;

	materialsNeeded.Glass -= 2;
	materialsNeeded.Fruit -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AutomobileFactory::AutomobileFactory(Stockpile* Target, Market* market) : Factory(Target, market, "automobile", 16000) {
	materialsProduced.Automobiles += 3 * size;

	materialsNeeded.Lumber -= 4;
	materialsNeeded.Iron -= 4;
	materialsNeeded.Rubber -= 1;
	materialsNeeded.Glass -= 1;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

Shipyard::Shipyard(Stockpile* Target, Market* market) : Factory(Target, market, "merchant ship", 19000) {
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

LumberMill::LumberMill(Stockpile* Target, Market* market) : Factory(Target, market, "lumber", 10000) {
	materialsProduced.Lumber += 15 * size;

	materialsNeeded.Timber -= 10;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

CementFactory::CementFactory(Stockpile* Target, Market* market) : Factory(Target, market, "cement", 12000) {
	materialsProduced.Cement += 10 * size;

	materialsNeeded.Coal -= 4;

	confirmMaterials();
}

ClothesFactory::ClothesFactory(Stockpile* Target, Market* market) : Factory(Target, market, "clothes", 11000) {
	materialsProduced.Clothes += 10 * size;

	materialsNeeded.Cotton -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

RadioFactory::RadioFactory(Stockpile* Target, Market* market) : Factory(Target, market, "radio", 15000) {
	materialsProduced.Radios += 5 * size;

	materialsNeeded.Electric_gear -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

TelephoneFactory::TelephoneFactory(Stockpile* Target, Market* market) : Factory(Target, market, "telephone", 16000) {
	materialsProduced.Telephones += 5 * size;

	materialsNeeded.Electric_gear -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

GlassFactory::GlassFactory(Stockpile* Target, Market* market) : Factory(Target, market, "glass", 8000) {
	materialsProduced.Glass += 8 * size;

	materialsNeeded.Coal -= 4;

	confirmMaterials();
}

PaperMill::PaperMill(Stockpile* Target, Market* market) : Factory(Target, market, "paper", 8000) {
	materialsProduced.Paper += 20 * size;

	materialsNeeded.Lumber -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

FurnitureFactory::FurnitureFactory(Stockpile* Target, Market* market) : Factory(Target, market, "furniture", 12000) {
	materialsProduced.Furniture += 7 * size;

	materialsNeeded.Lumber -= 10;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

FuelRefinery::FuelRefinery(Stockpile* Target, Market* market) : Factory(Target, market, "fuel", 22000) {
	materialsProduced.Fuel += 8 * size;

	materialsNeeded.Oil -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AmmunitionFactory::AmmunitionFactory(Stockpile* Target, Market* market) : Factory(Target, market, "ammunition", 14000) {
	materialsProduced.Ammunition += 5 * size;

	materialsNeeded.Iron -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

PlaneFactory::PlaneFactory(Stockpile* Target, Market* market) : Factory(Target, market, "plane", 24000) {
	materialsProduced.Planes += 2 * size;

	materialsNeeded.Lumber -= 4;
	materialsNeeded.Iron -= 1;
	materialsNeeded.Rubber -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ExplosivesFactory::ExplosivesFactory(Stockpile* Target, Market* market) : Factory(Target, market, "explosives", 13000) {
	materialsProduced.Explosives += 6 * size;

	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

SmallArmsFactory::SmallArmsFactory(Stockpile* Target, Market* market) : Factory(Target, market, "small arms", 16000) {
	materialsProduced.Small_arms += 15 * size;

	materialsNeeded.Iron -= 8;
	materialsNeeded.Lumber -= 8;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ArtilleryFactory::ArtilleryFactory(Stockpile* Target, Market* market) : Factory(Target, market, "artillery", 19000) {
	materialsProduced.Artillery += 3 * size;

	materialsNeeded.Iron -= 3;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

TankFactory::TankFactory(Stockpile* Target, Market* market) : Factory(Target, market, "tank", 30000) {
	materialsProduced.Tanks += 1 * size;

	materialsNeeded.Iron -= 4;
	materialsNeeded.Artillery -= 1;
	materialsNeeded.Small_arms -= 2;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

AirshipFactory::AirshipFactory(Stockpile* Target, Market* market) : Factory(Target, market, "airship", 40000) {
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

MachinePartFactory::MachinePartFactory(Stockpile* Target, Market* market) : Factory(Target, market, "machine parts", 20000) {
	materialsProduced.Machine_parts += 6 * size;

	materialsNeeded.Iron -= 12;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

ElectricGearFactory::ElectricGearFactory(Stockpile* Target, Market* market) : Factory(Target, market, "electric gear", 19000) {
	materialsProduced.Electric_gear += 8 * size;

	materialsNeeded.Iron -= 4;
	materialsNeeded.Coal -= 2;

	confirmMaterials();
}

SyntheticRubberRefinery::SyntheticRubberRefinery(Stockpile* Target, Market* market) : Factory(Target, market, "rubber", 14000) {
	materialsProduced.Rubber += 6 * size;

	materialsNeeded.Coal -= 5;

	confirmMaterials();
}
