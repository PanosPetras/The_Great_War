#include "PlayerController.h"
#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"


OpenFactoryScreen::OpenFactoryScreen(SDL_Renderer* r, int id, PlayerController* PC, std::function<void()> fp = NULL) : Screen(r){
	bHasBackground = false;

	int Width = GetWindowWidth(), Height = GetWindowHeight();

	ImageArr[0] = new Image(renderer, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	LabelArr[0] = new Label(renderer, "Open Factory", 32, int(Width * 0.45), int(Height * 0.22), 0, 0, 0);
	std::string lbl1txt = "Current Funds: " + std::to_string(PC->CountriesArr.at(PC->player_index)->Stock.Money);
	LabelArr[1] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.35), 0, 0, 0);
	lbl1txt = "Factory cost: " + std::to_string(10);
	LabelArr[2] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.41), 0, 0, 0);

	InputDrawableArr[0] = new Button(r, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Back", 32, [this]{ Close();});
	InputDrawableArr[1] = new Button(r, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Confirm", 32, [this]{ BuildFactory();});
	InputDrawableArr[2] = new Button(r, int(Width * 0.3), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/lumber", [this]{ FactoryTypeLumber();});
	InputDrawableArr[3] = new Button(r, int(Width * 0.35), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/glass", [this]{ FactoryTypeGlass();});
	InputDrawableArr[4] = new Button(r, int(Width * 0.4), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/canned food", [this]{ FactoryTypeFood();});
	InputDrawableArr[5] = new Button(r, int(Width * 0.45), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/Clothes", [this]{ FactoryTypeClothes();});
	InputDrawableArr[6] = new Button(r, int(Width * 0.3), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/liquor", [this]{ FactoryTypeLiquor();});
	InputDrawableArr[7] = new Button(r, int(Width * 0.35), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/furniture", [this]{ FactoryTypeFurniture();});
	InputDrawableArr[8] = new Button(r, int(Width * 0.4), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/automobile", [this]{ FactoryTypeAutomobile();});
	InputDrawableArr[9] = new Button(r, int(Width * 0.45), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/paper", [this]{ FactoryTypePaper();});
	InputDrawableArr[10] = new Button(r, int(Width * 0.3), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/telephone", [this]{ FactoryTypeTelephone();});
	InputDrawableArr[11] = new Button(r, int(Width * 0.35), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/radio", [this]{ FactoryTypeRadio();});
	InputDrawableArr[12] = new Button(r, int(Width * 0.4), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/machine parts", [this]{ FactoryTypeMachineParts();});
	InputDrawableArr[13] = new Button(r, int(Width * 0.45), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/electric gear", [this]{ FactoryTypeElectricGear();});
	InputDrawableArr[14] = new Button(r, int(Width * 0.3), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/fuel", [this]{ FactoryTypeFuel();});
	InputDrawableArr[15] = new Button(r, int(Width * 0.35), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/cement", [this]{ FactoryTypeCement();});
	InputDrawableArr[16] = new Button(r, int(Width * 0.4), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/merchant ship", [this]{ FactoryTypeMerchantShip();});
	InputDrawableArr[17] = new Button(r, int(Width * 0.45), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/small arms", [this]{ FactoryTypeSmallArms();});
	InputDrawableArr[18] = new Button(r, int(Width * 0.3), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/ammunition", [this]{ FactoryTypeAmmunition();});
	InputDrawableArr[19] = new Button(r, int(Width * 0.35), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/artillery", [this]{ FactoryTypeArtillery();});
	InputDrawableArr[20] = new Button(r, int(Width * 0.4), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/explosives", [this]{ FactoryTypeExplosives();});
	InputDrawableArr[21] = new Button(r, int(Width * 0.45), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/tank", [this]{ FactoryTypeTank();});
	InputDrawableArr[22] = new Button(r, int(Width * 0.3), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/airship", [this]{ FactoryTypeAirship();});
	InputDrawableArr[23] = new Button(r, int(Width * 0.35), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/plane", [this]{ FactoryTypeAirship();});

	InputDrawableArrtop = 24;
	LabelArrtop = 3;
	ImageArrtop = 1;
	bHasBackground = false;
	QuitFunc = fp;
	PCref = PC;
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
	Factory* NF;
	switch (type){
		case 'a':
			NF = new LumberMill(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'b':
			NF = new GlassFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'c':
			NF = new CanningFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'd':
			NF = new ClothesFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'e':
			NF = new LiquorDistillery(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'f':
			NF = new FurnitureFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'g':
			NF = new AutomobileFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'h':
			NF = new PaperMill(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'i':
			NF = new TelephoneFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'j':
			NF = new RadioFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'k':
			NF = new MachinePartFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'l':
			NF = new ElectricGearFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'm':
			NF = new FuelRefinery(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'n':
			NF = new CementFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'o':
			NF = new Shipyard(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'p':
			NF = new SmallArmsFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'q':
			NF = new AmmunitionFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'r':
			NF = new ArtilleryFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 's':
			NF = new ExplosivesFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 't':
			NF = new TankFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'u':
			NF = new AirshipFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		case 'v':
			NF = new PlaneFactory(&PCref->CountriesArr.at(PCref->player_index)->Stock, &PCref->WorldMarket);
			break;
		default:
			return;
			break;
	}

	if (PCref->CountriesArr.at(PCref->player_index)->Stock.Money >= NF->cost) {
		PCref->StatesArr[index]->AddFactory(NF);
		PCref->CountriesArr.at(PCref->player_index)->Stock.Money -= NF->cost;
		
		QuitFunc();
	}
}

void OpenFactoryScreen::Close(){
	QuitFunc();
}
