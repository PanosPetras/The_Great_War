#ifndef FACTORY_H
#define FACTORY_H

#pragma once
#include <string>
#include "Stockpile.h"
#include "Market.h"

struct Factory {
public:
	std::string Type;
	int cost;
	int size;

	Factory(Stockpile* Target, Market* market, std::string arg, int c);
	~Factory();

	virtual void Tick();

	void ChangeOwner(Stockpile* NewStockpile);

protected:
	Stockpile materialsNeeded;
	Stockpile materialsProduced;
	Stockpile* TargetStockpile;

	Market* TargetMarket;

	void confirmMaterials();
};

struct CanningFactory : public Factory {
public:
	CanningFactory(Stockpile* Target);
};

struct LiquorDistillery : public Factory {
public:
	LiquorDistillery(Stockpile* Target);
};

struct AutomobileFactory : public Factory {
public:
	AutomobileFactory(Stockpile* Target);
};

struct Shipyard : public Factory {
public:
	Shipyard(Stockpile* Target);

	void Tick() override;

private:
	int days;
	int daysToProduce;
};

struct LumberMill : public Factory {
public:
	LumberMill(Stockpile* Target);
};

struct CementFactory : public Factory {
public:
	CementFactory(Stockpile* Target);
};

struct ClothesFactory : public Factory {
public:
	ClothesFactory(Stockpile* Target);
};

struct RadioFactory : public Factory {
public:
	RadioFactory(Stockpile* Target);
};

struct TelephoneFactory : public Factory {
public:
	TelephoneFactory(Stockpile* Target);
};

struct GlassFactory : public Factory {
public:
	GlassFactory(Stockpile* Target);
};

struct PaperMill : public Factory {
public:
	PaperMill(Stockpile* Target);
};

struct FurnitureFactory : public Factory {
public:
	FurnitureFactory(Stockpile* Target);
};

struct FuelRefinery : public Factory {
public:
	FuelRefinery(Stockpile* Target);
};

struct AmmunitionFactory : public Factory {
public:
	AmmunitionFactory(Stockpile* Target);
};

struct PlaneFactory : public Factory {
public:
	PlaneFactory(Stockpile* Target);
};

struct ExplosivesFactory : public Factory {
public:
	ExplosivesFactory(Stockpile* Target);
};

struct SmallArmsFactory : public Factory {
public:
	SmallArmsFactory(Stockpile* Target);
};

struct ArtilleryFactory : public Factory {
public:
	ArtilleryFactory(Stockpile* Target);
};

struct TankFactory : public Factory {
public:
	TankFactory(Stockpile* Target);
};

struct AirshipFactory : public Factory {
public:
	AirshipFactory(Stockpile* Target);

	void Tick() override;

private:
	int days;
	int daysToProduce;
};

struct MachinePartFactory : public Factory {
public:
	MachinePartFactory(Stockpile* Target);
};

struct ElectricGearFactory : public Factory {
public:
	ElectricGearFactory(Stockpile* Target);
};

struct SyntheticRubberRefinery : public Factory {
public:
	SyntheticRubberRefinery(Stockpile* Target);
};
#endif