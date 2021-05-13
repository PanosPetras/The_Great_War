#include "Factory.h"

void Factory::Tick(){
}

void Factory::ChangeOwner(Stockpile* NewStockpile){
	TargetStockpile = NewStockpile;
}

CanningFactory::CanningFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "canned food";
}

void CanningFactory::Tick(){
	TargetStockpile->Canned_food += 10 * size;
	TargetStockpile->Iron -= 2;
	TargetStockpile->Grain -= 3;
	TargetStockpile->Coal -= 2;
}

LiquorDistillery::LiquorDistillery(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "liquor";
}

void LiquorDistillery::Tick(){
	TargetStockpile->Liquor += 10 * size;
	TargetStockpile->Glass -= 2;
	TargetStockpile->Fruit -= 4;
	TargetStockpile->Coal -= 2;
}

AutomobileFactory::AutomobileFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "automobile";
}

void AutomobileFactory::Tick(){
	TargetStockpile->Automobiles += 3 * size;
	TargetStockpile->Lumber -= 4;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Rubber -= 1;
	TargetStockpile->Glass -= 1;
	TargetStockpile->Coal -= 2;
}

Shipyard::Shipyard(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "merchant ship";
}

void Shipyard::Tick(){
}

LumberMill::LumberMill(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "lumber";
}

void LumberMill::Tick(){
	TargetStockpile->Lumber += 15 * size;
	TargetStockpile->Timber -= 10;
	TargetStockpile->Coal -= 2;
}

CementFactory::CementFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "cement";
}

void CementFactory::Tick(){
	TargetStockpile->Cement += 10 * size;
	TargetStockpile->Coal -= 4;
}

ClothesFactory::ClothesFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "clothes";
}

void ClothesFactory::Tick(){
	TargetStockpile->Clothes += 10 * size;
	TargetStockpile->Cotton -= 8;
	TargetStockpile->Coal -= 2;
}

RadioFactory::RadioFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "radio";
}

void RadioFactory::Tick(){
	TargetStockpile->Radios += 5 * size;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Coal -= 2;
}

TelephoneFactory::TelephoneFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "telephone";
}

void TelephoneFactory::Tick(){
	TargetStockpile->Telephones += 5 * size;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Coal -= 2;
}

GlassFactory::GlassFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "glass";
}

void GlassFactory::Tick(){
	TargetStockpile->Glass += 8 * size;
	TargetStockpile->Coal -= 4;
}

PaperMill::PaperMill(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "paper";
}

void PaperMill::Tick(){
	TargetStockpile->Paper += 20 * size;
	TargetStockpile->Lumber -= 8;
	TargetStockpile->Coal -= 2;
}

FurnitureFactory::FurnitureFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "furniture";
}

void FurnitureFactory::Tick(){
	TargetStockpile->Furniture += 7 * size;
	TargetStockpile->Lumber -= 10;
	TargetStockpile->Coal -= 2;
}

FuelRefinery::FuelRefinery(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "fuel";
}

void FuelRefinery::Tick(){
	TargetStockpile->Fuel += 8 * size;
	TargetStockpile->Oil -= 4;
	TargetStockpile->Coal -= 2;
}

AmmunitionFactory::AmmunitionFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "ammunition";
}

void AmmunitionFactory::Tick(){
	TargetStockpile->Ammunition += 5 * size;
	TargetStockpile->Iron -= 3;
	TargetStockpile->Coal -= 2;
}

PlaneFactory::PlaneFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "plane";
}

void PlaneFactory::Tick(){
	TargetStockpile->Planes += 2 * size;
	TargetStockpile->Lumber -= 4;
	TargetStockpile->Iron -= 1;
	TargetStockpile->Rubber -= 2;
	TargetStockpile->Coal -= 2;
}

ExplosivesFactory::ExplosivesFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "explosives";
}

void ExplosivesFactory::Tick(){
	TargetStockpile->Explosives += 6 * size;
	TargetStockpile->Coal -= 2;
}

SmallArmsFactory::SmallArmsFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "small arms";
}

void SmallArmsFactory::Tick(){
	TargetStockpile->Small_arms += 15 * size;
	TargetStockpile->Iron -= 8;
	TargetStockpile->Lumber -= 8;
	TargetStockpile->Coal -= 2;
}

ArtilleryFactory::ArtilleryFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "artillery";
}

void ArtilleryFactory::Tick(){
	TargetStockpile->Artillery += 3 * size;
	TargetStockpile->Iron -= 3;
	TargetStockpile->Coal -= 2;
}

TankFactory::TankFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "tank";
}

void TankFactory::Tick(){
	TargetStockpile->Tanks += 1 * size;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Artillery -= 1;
	TargetStockpile->Small_arms -= 2;
	TargetStockpile->Coal -= 2;
}

AirshipFactory::AirshipFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "airship";
	days = 0;
}

void AirshipFactory::Tick(){
	days++;
	if (days == 153) {
		TargetStockpile->Airship += 1 * size;
		days = 0;
	}
	TargetStockpile->Iron -= 10;
	TargetStockpile->Electric_gear -= 2;
	TargetStockpile->Cotton -= 12;
	TargetStockpile->Coal -= 2;
}

MachinePartFactory::MachinePartFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "machine parts";
}

void MachinePartFactory::Tick(){
	TargetStockpile->Machine_parts += 6 * size;
	TargetStockpile->Iron -= 12;
	TargetStockpile->Coal -= 2;
}

ElectricGearFactory::ElectricGearFactory(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "electric gear";
}

void ElectricGearFactory::Tick(){
	TargetStockpile->Electric_gear += 8 * size;
	TargetStockpile->Iron -= 4;
	TargetStockpile->Coal -= 2;
}

SyntheticRubberRefinery::SyntheticRubberRefinery(Stockpile* Target){
	size = 1;
	ChangeOwner(Target);
	Type = "rubber";
}

void SyntheticRubberRefinery::Tick(){
	TargetStockpile->Rubber += 6 * size;
	TargetStockpile->Coal -= 5;
}