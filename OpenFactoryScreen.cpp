#include "ScreenList.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"

OpenFactoryScreen::OpenFactoryScreen(SDL_Renderer* r, int id, PlayerController* PC, std::function<void()> quitfunc) : Screen(r, quitfunc){
    PCref = PC;
    auto [Width, Height] = GetWindowDimensions();

    AddImage<Image>(renderer, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
    AddLabel<Label>(renderer, "Open Factory", 32, int(Width * 0.45), int(Height * 0.22), 0, 0, 0);
    std::string lbl1txt = "Current Funds: " + std::to_string(PC->CountriesArr.at(PC->player_index)->Stock.Money);
    AddLabel<Label>(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.35), 0, 0, 0);
    lbl1txt = "Factory cost: " + std::to_string(10);
    AddLabel<Label>(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.41), 0, 0, 0);

    AddDrawable<Button>(r, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Close();});
    AddDrawable<Button>(r, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Confirm", 32, [this]{ BuildFactory();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/lumber", [this]{ FactoryTypeLumber();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/glass", [this]{ FactoryTypeGlass();});
    AddDrawable<Button>(r, int(Width * 0.4), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/canned food", [this]{ FactoryTypeFood();});
    AddDrawable<Button>(r, int(Width * 0.45), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/Clothes", [this]{ FactoryTypeClothes();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/liquor", [this]{ FactoryTypeLiquor();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/furniture", [this]{ FactoryTypeFurniture();});
    AddDrawable<Button>(r, int(Width * 0.4), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/automobile", [this]{ FactoryTypeAutomobile();});
    AddDrawable<Button>(r, int(Width * 0.45), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/paper", [this]{ FactoryTypePaper();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/telephone", [this]{ FactoryTypeTelephone();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/radio", [this]{ FactoryTypeRadio();});
    AddDrawable<Button>(r, int(Width * 0.4), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/machine parts", [this]{ FactoryTypeMachineParts();});
    AddDrawable<Button>(r, int(Width * 0.45), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/electric gear", [this]{ FactoryTypeElectricGear();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/fuel", [this]{ FactoryTypeFuel();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/cement", [this]{ FactoryTypeCement();});
    AddDrawable<Button>(r, int(Width * 0.4), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/merchant ship", [this]{ FactoryTypeMerchantShip();});
    AddDrawable<Button>(r, int(Width * 0.45), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/small arms", [this]{ FactoryTypeSmallArms();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/ammunition", [this]{ FactoryTypeAmmunition();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/artillery", [this]{ FactoryTypeArtillery();});
    AddDrawable<Button>(r, int(Width * 0.4), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/explosives", [this]{ FactoryTypeExplosives();});
    AddDrawable<Button>(r, int(Width * 0.45), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/tank", [this]{ FactoryTypeTank();});
    AddDrawable<Button>(r, int(Width * 0.3), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/airship", [this]{ FactoryTypeAirship();});
    AddDrawable<Button>(r, int(Width * 0.35), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/plane", [this]{ FactoryTypePlane();});

    index = id;
}

void OpenFactoryScreen::FactoryTypeLumber(){
    LumberMill F(nullptr, nullptr);
    FactoryType('a', F.cost);
}

void OpenFactoryScreen::FactoryTypeGlass(){
    GlassFactory F(nullptr, nullptr);
    FactoryType('b', F.cost);
}

void OpenFactoryScreen::FactoryTypeFood(){
    CanningFactory F(nullptr, nullptr);
    FactoryType('c', F.cost);
}

void OpenFactoryScreen::FactoryTypeClothes(){
    ClothesFactory F(nullptr, nullptr);
    FactoryType('d', F.cost);
}

void OpenFactoryScreen::FactoryTypeLiquor(){
    LiquorDistillery F(nullptr, nullptr);
    FactoryType('e', F.cost);
}

void OpenFactoryScreen::FactoryTypeFurniture(){
    FurnitureFactory F(nullptr, nullptr);
    FactoryType('f', F.cost);
}

void OpenFactoryScreen::FactoryTypeAutomobile(){
    AutomobileFactory F(nullptr, nullptr);
    FactoryType('g', F.cost);
}

void OpenFactoryScreen::FactoryTypePaper(){
    PaperMill F(nullptr, nullptr);
    FactoryType('h', F.cost);
}

void OpenFactoryScreen::FactoryTypeTelephone(){
    TelephoneFactory F(nullptr, nullptr);
    FactoryType('i', F.cost);
}

void OpenFactoryScreen::FactoryTypeRadio(){
    RadioFactory F(nullptr, nullptr);
    FactoryType('j', F.cost);
}

void OpenFactoryScreen::FactoryTypeMachineParts(){
    MachinePartFactory F(nullptr, nullptr);
    FactoryType('k', F.cost);
}

void OpenFactoryScreen::FactoryTypeElectricGear(){
    ElectricGearFactory F(nullptr, nullptr);
    FactoryType('l', F.cost);
}

void OpenFactoryScreen::FactoryTypeFuel(){
    FuelRefinery F(nullptr, nullptr);
    FactoryType('m', F.cost);
}

void OpenFactoryScreen::FactoryTypeCement(){
    CementFactory F(nullptr, nullptr);
    FactoryType('n', F.cost);
}

void OpenFactoryScreen::FactoryTypeMerchantShip(){
    Shipyard F(nullptr, nullptr);
    FactoryType('o', F.cost);
}

void OpenFactoryScreen::FactoryTypeSmallArms(){
    SmallArmsFactory F(nullptr, nullptr);
    FactoryType('p', F.cost);
}

void OpenFactoryScreen::FactoryTypeAmmunition(){
    AmmunitionFactory F(nullptr, nullptr);
    FactoryType('q', F.cost);
}

void OpenFactoryScreen::FactoryTypeArtillery(){
    ArtilleryFactory F(nullptr, nullptr);
    FactoryType('r', F.cost);
}

void OpenFactoryScreen::FactoryTypeExplosives(){
    ExplosivesFactory F(nullptr, nullptr);
    FactoryType('s', F.cost);
}

void OpenFactoryScreen::FactoryTypeTank(){
    TankFactory F(nullptr, nullptr);
    FactoryType('t', F.cost);
}

void OpenFactoryScreen::FactoryTypeAirship(){
    AirshipFactory F(nullptr, nullptr);
    FactoryType('u', F.cost);
}

void OpenFactoryScreen::FactoryTypePlane(){
    PlaneFactory F(nullptr, nullptr);
    FactoryType('v', F.cost);
}

void OpenFactoryScreen::FactoryType(char t, int cost){
    type = t;
    std::string txt = "Factory cost: " + std::to_string(cost);
    LabelArr[2]->ChangeText(txt.c_str());
}

void OpenFactoryScreen::BuildFactory(){
    std::unique_ptr<Factory> NF;
    switch (type){
        case 'a':
                NF = std::make_unique<LumberMill>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'b':
                NF = std::make_unique<GlassFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'c':
                NF = std::make_unique<CanningFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'd':
                NF = std::make_unique<ClothesFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'e':
                NF = std::make_unique<LiquorDistillery>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'f':
                NF = std::make_unique<FurnitureFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'g':
                NF = std::make_unique<AutomobileFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'h':
                NF = std::make_unique<PaperMill>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'i':
                NF = std::make_unique<TelephoneFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'j':
                NF = std::make_unique<RadioFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'k':
                NF = std::make_unique<MachinePartFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'l':
                NF = std::make_unique<ElectricGearFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'm':
                NF = std::make_unique<FuelRefinery>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'n':
                NF = std::make_unique<CementFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'o':
                NF = std::make_unique<Shipyard>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'p':
                NF = std::make_unique<SmallArmsFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'q':
                NF = std::make_unique<AmmunitionFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'r':
                NF = std::make_unique<ArtilleryFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 's':
                NF = std::make_unique<ExplosivesFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 't':
                NF = std::make_unique<TankFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'u':
                NF = std::make_unique<AirshipFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        case 'v':
                NF = std::make_unique<PlaneFactory>(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
                break;
        default:
                return;
                break;
    }

    if (PCref->CountriesArr.at(PCref->player_index)->Stock.Money >= NF->cost){
        PCref->CountriesArr.at(PCref->player_index)->Stock.Money -= NF->cost;
        PCref->StatesArr[index].AddFactory(NF);

        QuitFunc();
    }
}

void OpenFactoryScreen::Close(){
    QuitFunc();
}
