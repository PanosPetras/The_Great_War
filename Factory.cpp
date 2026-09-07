#include "Factory.h"

Factory::Factory(FactoryType type, Stockpile* Target, Market* market) : Kind{KindOf(type)}, size{1}, TargetStockpile{Target}, TargetMarket{market}, days{0} {
    // Every batch the factory runs at once yields another copy of the output
    for(int batch = 0; batch < size; batch++) {
        materialsProduced += Kind.produces;
    }

    // The inputs leave the stockpile, so they are held as what they take away
    materialsNeeded -= Kind.consumes;

    confirmMaterials();
}

Factory::~Factory() {
    if(TargetMarket == nullptr) return;
    TargetMarket->Demand += materialsNeeded;
    TargetMarket->Supply -= materialsProduced;
}

void Factory::Tick() {
    *TargetStockpile += materialsNeeded;

    if(++days == Kind.daysToProduce) {
        days = 0;
        *TargetStockpile += materialsProduced;
    }
}

void Factory::ChangeOwner(Stockpile* NewStockpile) {
    TargetStockpile = NewStockpile;
}

void Factory::confirmMaterials() {
    if(TargetMarket == nullptr) return;
    TargetMarket->Demand -= materialsNeeded;
    TargetMarket->Supply += materialsProduced;
}
