#include "Factory.h"

void Factory::Tick(){
}

Factory::Factory(Stockpile* Target, std::string arg, int c = 500){
	cost = c;
	size = 1;
	ChangeOwner(Target);
	Type = arg;
}

void Factory::ChangeOwner(Stockpile* NewStockpile){
	TargetStockpile = NewStockpile;
}

CanningFactory::CanningFactory(Stockpile* Target) : Factory(Target, "canned food") {
}

void CanningFactory::Tick(){
	TargetStockpile->Canned_food += 10 * size;
	TargetStockpile->Iron -= 2;
	TargetStockpile->Grain -= 3;
	TargetStockpile->Coal -= 2;
}

LiquorDistillery::LiquorDistillery(Stockpile* Target) : Factory(Target, "liquor") {
}

void LiquorDistillery::Tick(){
	TargetStockpile->Liquor += 10 * size;
	TargetStockpile->Glass -= 2;
	TargetStockpile->Fruit -= 4;
	TargetStockpile->Coal -= 2;
}

AutomobileFactory::AutomobileFactory(Stockpile* Target) : Factory(Target, "automobile") {
}

void AutomobileFactory::Tick(){
	TargetStockpile->Automobiles += 3 * size;
	TargetStockpile->Lumber -= 4;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Rubber -= 1;
	TargetStockpile->Glass -= 1;
	TargetStockpile->Coal -= 2;
}

Shipyard::Shipyard(Stockpile* Target) : Factory(Target, "merchant ship") {
}

void Shipyard::Tick(){
}

LumberMill::LumberMill(Stockpile* Target) : Factory(Target, "lumber") {
}

void LumberMill::Tick(){
	TargetStockpile->Lumber += 15 * size;
	TargetStockpile->Timber -= 10;
	TargetStockpile->Coal -= 2;
}

CementFactory::CementFactory(Stockpile* Target) : Factory(Target, "cement") {
}

void CementFactory::Tick(){
	TargetStockpile->Cement += 10 * size;
	TargetStockpile->Coal -= 4;
}

ClothesFactory::ClothesFactory(Stockpile* Target) : Factory(Target, "clothes") {
}

void ClothesFactory::Tick(){
	TargetStockpile->Clothes += 10 * size;
	TargetStockpile->Cotton -= 8;
	TargetStockpile->Coal -= 2;
}

RadioFactory::RadioFactory(Stockpile* Target) : Factory(Target, "radio") {
}

void RadioFactory::Tick(){
	TargetStockpile->Radios += 5 * size;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Coal -= 2;
}

TelephoneFactory::TelephoneFactory(Stockpile* Target) : Factory(Target, "telephone") {
}

void TelephoneFactory::Tick(){
	TargetStockpile->Telephones += 5 * size;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Coal -= 2;
}

GlassFactory::GlassFactory(Stockpile* Target) : Factory(Target, "glass") {
}

void GlassFactory::Tick(){
	TargetStockpile->Glass += 8 * size;
	TargetStockpile->Coal -= 4;
}

PaperMill::PaperMill(Stockpile* Target) : Factory(Target, "paper") {
}

void PaperMill::Tick(){
	TargetStockpile->Paper += 20 * size;
	TargetStockpile->Lumber -= 8;
	TargetStockpile->Coal -= 2;
}

FurnitureFactory::FurnitureFactory(Stockpile* Target) : Factory(Target, "furniture") {
}

void FurnitureFactory::Tick(){
	TargetStockpile->Furniture += 7 * size;
	TargetStockpile->Lumber -= 10;
	TargetStockpile->Coal -= 2;
}

FuelRefinery::FuelRefinery(Stockpile* Target) : Factory(Target, "fuel") {
}

void FuelRefinery::Tick(){
	TargetStockpile->Fuel += 8 * size;
	TargetStockpile->Oil -= 4;
	TargetStockpile->Coal -= 2;
}

AmmunitionFactory::AmmunitionFactory(Stockpile* Target) : Factory(Target, "ammunition") {
}

void AmmunitionFactory::Tick(){
	TargetStockpile->Ammunition += 5 * size;
	TargetStockpile->Iron -= 3;
	TargetStockpile->Coal -= 2;
}

PlaneFactory::PlaneFactory(Stockpile* Target) : Factory(Target, "plane") {
}

void PlaneFactory::Tick(){
	TargetStockpile->Planes += 2 * size;
	TargetStockpile->Lumber -= 4;
	TargetStockpile->Iron -= 1;
	TargetStockpile->Rubber -= 2;
	TargetStockpile->Coal -= 2;
}

ExplosivesFactory::ExplosivesFactory(Stockpile* Target) : Factory(Target, "explosives") {
}

void ExplosivesFactory::Tick(){
	TargetStockpile->Explosives += 6 * size;
	TargetStockpile->Coal -= 2;
}

SmallArmsFactory::SmallArmsFactory(Stockpile* Target) : Factory(Target, "small arms") {
}

void SmallArmsFactory::Tick(){
	TargetStockpile->Small_arms += 15 * size;
	TargetStockpile->Iron -= 8;
	TargetStockpile->Lumber -= 8;
	TargetStockpile->Coal -= 2;
}

ArtilleryFactory::ArtilleryFactory(Stockpile* Target) : Factory(Target, "artillery") {
}

void ArtilleryFactory::Tick(){
	TargetStockpile->Artillery += 3 * size;
	TargetStockpile->Iron -= 3;
	TargetStockpile->Coal -= 2;
}

TankFactory::TankFactory(Stockpile* Target) : Factory(Target, "tank") {
}

void TankFactory::Tick(){
	TargetStockpile->Tanks += 1 * size;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Artillery -= 1;
	TargetStockpile->Small_arms -= 2;
	TargetStockpile->Coal -= 2;
}

AirshipFactory::AirshipFactory(Stockpile* Target) : Factory(Target, "airship") {
	days = 0;
}

void AirshipFactory::Tick(){
	if (days >= 153) {
		TargetStockpile->Airship += 1 * size;
		//days = 0;
	} else {
		days++;
	}
	TargetStockpile->Iron -= 10;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Cotton -= 12;
	TargetStockpile->Coal -= 2;
}

MachinePartFactory::MachinePartFactory(Stockpile* Target) : Factory(Target, "machine parts") {
}

void MachinePartFactory::Tick(){
	TargetStockpile->Machine_parts += 6 * size;
	TargetStockpile->Iron -= 12;
	TargetStockpile->Coal -= 2;
}

ElectricGearFactory::ElectricGearFactory(Stockpile* Target) : Factory(Target, "electric gear") {
}

void ElectricGearFactory::Tick(){
	TargetStockpile->Electric_gear += 8 * size;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Coal -= 2;
}

SyntheticRubberRefinery::SyntheticRubberRefinery(Stockpile* Target) : Factory(Target, "rubber") {
}

void SyntheticRubberRefinery::Tick(){
	TargetStockpile->Rubber += 6 * size;
	TargetStockpile->Coal -= 5;
}