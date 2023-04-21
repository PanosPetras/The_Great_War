#ifndef FACTORY_H
#define FACTORY_H

#pragma once
#include "Market.h"
#include "Stockpile.h"
#include <string>

struct Factory {
public:
    std::string Type;
    int cost;
    int size;

    Factory(Stockpile* Target, Market* market, std::string arg, int c);
    virtual ~Factory();

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
    CanningFactory(Stockpile* Target, Market* market);
};

struct LiquorDistillery : public Factory {
public:
    LiquorDistillery(Stockpile* Target, Market* market);
};

struct AutomobileFactory : public Factory {
public:
    AutomobileFactory(Stockpile* Target, Market* market);
};

struct Shipyard : public Factory {
public:
    Shipyard(Stockpile* Target, Market* market);

    void Tick() override;

private:
    int days;
    int daysToProduce;
};

struct LumberMill : public Factory {
public:
    LumberMill(Stockpile* Target, Market* market);
};

struct CementFactory : public Factory {
public:
    CementFactory(Stockpile* Target, Market* market);
};

struct ClothesFactory : public Factory {
public:
    ClothesFactory(Stockpile* Target, Market* market);
};

struct RadioFactory : public Factory {
public:
    RadioFactory(Stockpile* Target, Market* market);
};

struct TelephoneFactory : public Factory {
public:
    TelephoneFactory(Stockpile* Target, Market* market);
};

struct GlassFactory : public Factory {
public:
    GlassFactory(Stockpile* Target, Market* market);
};

struct PaperMill : public Factory {
public:
    PaperMill(Stockpile* Target, Market* market);
};

struct FurnitureFactory : public Factory {
public:
    FurnitureFactory(Stockpile* Target, Market* market);
};

struct FuelRefinery : public Factory {
public:
    FuelRefinery(Stockpile* Target, Market* market);
};

struct AmmunitionFactory : public Factory {
public:
    AmmunitionFactory(Stockpile* Target, Market* market);
};

struct PlaneFactory : public Factory {
public:
    PlaneFactory(Stockpile* Target, Market* market);
};

struct ExplosivesFactory : public Factory {
public:
    ExplosivesFactory(Stockpile* Target, Market* market);
};

struct SmallArmsFactory : public Factory {
public:
    SmallArmsFactory(Stockpile* Target, Market* market);
};

struct ArtilleryFactory : public Factory {
public:
    ArtilleryFactory(Stockpile* Target, Market* market);
};

struct TankFactory : public Factory {
public:
    TankFactory(Stockpile* Target, Market* market);
};

struct AirshipFactory : public Factory {
public:
    AirshipFactory(Stockpile* Target, Market* market);

    void Tick() override;

private:
    int days;
    int daysToProduce;
};

struct MachinePartFactory : public Factory {
public:
    MachinePartFactory(Stockpile* Target, Market* market);
};

struct ElectricGearFactory : public Factory {
public:
    ElectricGearFactory(Stockpile* Target, Market* market);
};

struct SyntheticRubberRefinery : public Factory {
public:
    SyntheticRubberRefinery(Stockpile* Target, Market* market);
};
#endif
