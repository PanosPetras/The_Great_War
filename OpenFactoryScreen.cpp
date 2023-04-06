#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"


OpenFactoryScreen::OpenFactoryScreen(SDL_Renderer* r, int id, PlayerController* PC, std::function<void()> fp = NULL) : Screen(r){
	bHasBackground = false;

	int Width = GetWindowWidth(), Height = GetWindowHeight();

	ImageArr[0] = new Image(renderer, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	LabelArr[0] = new Label(renderer, "Open Factory", 32, int(Width * 0.45), int(Height * 0.22), 0, 0, 0);
	std::string lbl1txt = "Current Funds: " + std::to_string(PC->CountriesArr.at(PC->player_index)->Stock.Money);
	LabelArr[1] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.35), 0, 0, 0);
	lbl1txt = "Factory cost: " + std::to_string(10);
	LabelArr[2] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.41), 0, 0, 0);

	auto conf = std::bind(&OpenFactoryScreen::Close, this);
	InputDrawableArr[0] = new Button(r, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Back", 32, conf);
	conf = std::bind(&OpenFactoryScreen::BuildFactory, this);
	InputDrawableArr[1] = new Button(r, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Confirm", 32, conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeLumber, this);
	InputDrawableArr[2] = new Button(r, int(Width * 0.3), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/lumber", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeGlass, this);
	InputDrawableArr[3] = new Button(r, int(Width * 0.35), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/glass", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFood, this);
	InputDrawableArr[4] = new Button(r, int(Width * 0.4), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/canned food", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeClothes, this);
	InputDrawableArr[5] = new Button(r, int(Width * 0.45), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/Clothes", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeLiquor, this);
	InputDrawableArr[6] = new Button(r, int(Width * 0.3), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/liquor", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFurniture, this);
	InputDrawableArr[7] = new Button(r, int(Width * 0.35), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/furniture", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAutomobile, this);
	InputDrawableArr[8] = new Button(r, int(Width * 0.4), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/automobile", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypePaper, this);
	InputDrawableArr[9] = new Button(r, int(Width * 0.45), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/paper", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeTelephone, this);
	InputDrawableArr[10] = new Button(r, int(Width * 0.3), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/telephone", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeRadio, this);
	InputDrawableArr[11] = new Button(r, int(Width * 0.35), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/radio", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeMachineParts, this);
	InputDrawableArr[12] = new Button(r, int(Width * 0.4), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/machine parts", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeElectricGear, this);
	InputDrawableArr[13] = new Button(r, int(Width * 0.45), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/electric gear", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFuel, this);
	InputDrawableArr[14] = new Button(r, int(Width * 0.3), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/fuel", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeCement, this);
	InputDrawableArr[15] = new Button(r, int(Width * 0.35), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/cement", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeMerchantShip, this);
	InputDrawableArr[16] = new Button(r, int(Width * 0.4), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/merchant ship", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeSmallArms, this);
	InputDrawableArr[17] = new Button(r, int(Width * 0.45), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/small arms", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAmmunition, this);
	InputDrawableArr[18] = new Button(r, int(Width * 0.3), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/ammunition", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeArtillery, this);
	InputDrawableArr[19] = new Button(r, int(Width * 0.35), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/artillery", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeExplosives, this);
	InputDrawableArr[20] = new Button(r, int(Width * 0.4), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/explosives", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeTank, this);
	InputDrawableArr[21] = new Button(r, int(Width * 0.45), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/tank", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAirship, this);
	InputDrawableArr[22] = new Button(r, int(Width * 0.3), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/airship", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypePlane, this);
	InputDrawableArr[23] = new Button(r, int(Width * 0.35), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/plane", conf);

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
