#ifndef FACTORY_H
#define FACTORY_H
#include <string>
#include "Stockpile.h"

struct Factory {
public:
	int size;
	int cost;
	virtual void Tick();
	std::string Type;

	Factory(Stockpile* Target, std::string arg, int c);
	void ChangeOwner(Stockpile* NewStockpile);
	Stockpile* TargetStockpile;
};

struct CanningFactory : public Factory{
public:
	CanningFactory(Stockpile* Target);
	void Tick() override;
};

struct LiquorDistillery : public Factory {
public:
	LiquorDistillery(Stockpile* Target);
	void Tick() override;
};

struct AutomobileFactory : public Factory {
public:
	AutomobileFactory(Stockpile* Target);
	void Tick() override;
};

struct Shipyard : public Factory {
public:
	Shipyard(Stockpile* Target);
	void Tick() override;
};

struct LumberMill : public Factory {
public:
	LumberMill(Stockpile* Target);
	void Tick() override;
};

struct CementFactory : public Factory {
public:
	CementFactory(Stockpile* Target);
	void Tick() override;
};

struct ClothesFactory : public Factory {
public:
	ClothesFactory(Stockpile* Target);
	void Tick() override;
};

struct RadioFactory : public Factory {
public:
	RadioFactory(Stockpile* Target);
	void Tick() override;
};

struct TelephoneFactory : public Factory {
public:
	TelephoneFactory(Stockpile* Target);
	void Tick() override;
};

struct GlassFactory : public Factory {
public:
	GlassFactory(Stockpile* Target);
	void Tick() override;
};

struct PaperMill : public Factory {
public:
	PaperMill(Stockpile* Target);
	void Tick() override;
};

struct FurnitureFactory : public Factory {
public:
	FurnitureFactory(Stockpile* Target);
	void Tick() override;
};

struct FuelRefinery : public Factory {
public:
	FuelRefinery(Stockpile* Target);
	void Tick() override;
};

struct AmmunitionFactory : public Factory {
public:
	AmmunitionFactory(Stockpile* Target);
	void Tick() override;
};

struct PlaneFactory : public Factory {
public:
	PlaneFactory(Stockpile* Target);
	void Tick() override;
};

struct ExplosivesFactory : public Factory {
public:
	ExplosivesFactory(Stockpile* Target);
	void Tick() override;
};

struct SmallArmsFactory : public Factory {
public:
	SmallArmsFactory(Stockpile* Target);
	void Tick() override;
};

struct ArtilleryFactory : public Factory {
public:
	ArtilleryFactory(Stockpile* Target);
	void Tick() override;
};

struct TankFactory : public Factory {
public:
	TankFactory(Stockpile* Target);
	void Tick() override;
};

struct AirshipFactory : public Factory {
public:
	AirshipFactory(Stockpile* Target);

	int days;

	void Tick() override;
};

struct MachinePartFactory : public Factory {
public:
	MachinePartFactory(Stockpile* Target);
	void Tick() override;
};

struct ElectricGearFactory : public Factory {
public:
	ElectricGearFactory(Stockpile* Target);
	void Tick() override;
};

struct SyntheticRubberRefinery : public Factory {
public:
	SyntheticRubberRefinery(Stockpile* Target);
	void Tick() override;
};
#endif