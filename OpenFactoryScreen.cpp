#include "ScreenList.h"


OpenFactoryScreen::OpenFactoryScreen(SDL_Renderer* r, int Width, int Height, int id, PlayerController* PC, std::function<void()> fp = NULL) : Screen(r, Width, Height){
	bHasBackground = false;
	ImageArr[0] = new Image(renderer, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	LabelArr[0] = new Label(renderer, "Open Factory", 32, int(Width * 0.45), int(Height * 0.22), 0, 0, 0);
	std::string lbl1txt = "Current Funds: " + std::to_string(PC->CountriesArr[PC->player_index]->Stock.Money);
	LabelArr[1] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.35), 0, 0, 0);
	lbl1txt = "Factory cost: " + std::to_string(10);
	LabelArr[2] = new Label(renderer, lbl1txt.c_str(), 32, int(Width * 0.55), int(Height * 0.41), 0, 0, 0);

	auto conf = std::bind(&OpenFactoryScreen::Close, this);
	ButtonArr[0] = new Button(r, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Back", conf);
	conf = std::bind(&OpenFactoryScreen::BuildFactory, this);
	ButtonArr[1] = new Button(r, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Buttons/Menus/Confirm", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeLumber, this);
	ButtonArr[2] = new Button(r, int(Width * 0.3), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/lumber", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeGlass, this);
	ButtonArr[3] = new Button(r, int(Width * 0.35), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/glass", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFood, this);
	ButtonArr[4] = new Button(r, int(Width * 0.4), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/canned food", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeClothes, this);
	ButtonArr[5] = new Button(r, int(Width * 0.45), int(Height * 0.3), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/Clothes", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeLiquor, this);
	ButtonArr[6] = new Button(r, int(Width * 0.3), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/liquor", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFurniture, this);
	ButtonArr[7] = new Button(r, int(Width * 0.35), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/furniture", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAutomobile, this);
	ButtonArr[8] = new Button(r, int(Width * 0.4), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/automobile", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypePaper, this);
	ButtonArr[9] = new Button(r, int(Width * 0.45), int(Height * 0.36), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/paper", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeTelephone, this);
	ButtonArr[10] = new Button(r, int(Width * 0.3), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/telephone", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeRadio, this);
	ButtonArr[11] = new Button(r, int(Width * 0.35), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/radio", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeMachineParts, this);
	ButtonArr[12] = new Button(r, int(Width * 0.4), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/machine parts", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeElectricGear, this);
	ButtonArr[13] = new Button(r, int(Width * 0.45), int(Height * 0.42), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/electric gear", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeFuel, this);
	ButtonArr[14] = new Button(r, int(Width * 0.3), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/fuel", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeCement, this);
	ButtonArr[15] = new Button(r, int(Width * 0.35), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/cement", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeMerchantShip, this);
	ButtonArr[16] = new Button(r, int(Width * 0.4), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/merchant ship", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeSmallArms, this);
	ButtonArr[17] = new Button(r, int(Width * 0.45), int(Height * 0.48), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/small arms", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAmmunition, this);
	ButtonArr[18] = new Button(r, int(Width * 0.3), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/ammunition", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeArtillery, this);
	ButtonArr[19] = new Button(r, int(Width * 0.35), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/artillery", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeExplosives, this);
	ButtonArr[20] = new Button(r, int(Width * 0.4), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/explosives", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeTank, this);
	ButtonArr[21] = new Button(r, int(Width * 0.45), int(Height * 0.54), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/tank", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypeAirship, this);
	ButtonArr[22] = new Button(r, int(Width * 0.3), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/airship", conf);
	conf = std::bind(&OpenFactoryScreen::FactoryTypePlane, this);
	ButtonArr[23] = new Button(r, int(Width * 0.35), int(Height * 0.6), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/plane", conf);
	ButtonArrtop = 24;
	LabelArrtop = 3;
	ImageArrtop = 1;
	SliderArrtop = 0;
	bHasBackground = false;
	QuitFunc = fp;
	PCref = PC;
	index = id;
}

void OpenFactoryScreen::FactoryTypeLumber(){
	LumberMill F(nullptr);
	FactoryType('a', F.cost);
}

void OpenFactoryScreen::FactoryTypeGlass(){
	GlassFactory F(nullptr);
	FactoryType('b', F.cost);
}

void OpenFactoryScreen::FactoryTypeFood(){
	CanningFactory F(nullptr);
	FactoryType('c', F.cost);
}

void OpenFactoryScreen::FactoryTypeClothes(){
	ClothesFactory F(nullptr);
	FactoryType('d', F.cost);
}

void OpenFactoryScreen::FactoryTypeLiquor(){
	LiquorDistillery F(nullptr);
	FactoryType('e', F.cost);
}

void OpenFactoryScreen::FactoryTypeFurniture(){
	FurnitureFactory F(nullptr);
	FactoryType('f', F.cost);
}

void OpenFactoryScreen::FactoryTypeAutomobile(){
	AutomobileFactory F(nullptr);
	FactoryType('g', F.cost);
}

void OpenFactoryScreen::FactoryTypePaper(){
	PaperMill F(nullptr);
	FactoryType('h', F.cost);
}

void OpenFactoryScreen::FactoryTypeTelephone(){
	TelephoneFactory F(nullptr);
	FactoryType('i', F.cost);
}

void OpenFactoryScreen::FactoryTypeRadio(){
	RadioFactory F(nullptr);
	FactoryType('j', F.cost);
}

void OpenFactoryScreen::FactoryTypeMachineParts(){
	MachinePartFactory F(nullptr);
	FactoryType('k', F.cost);
}

void OpenFactoryScreen::FactoryTypeElectricGear(){
	ElectricGearFactory F(nullptr);
	FactoryType('l', F.cost);
}

void OpenFactoryScreen::FactoryTypeFuel(){
	FuelRefinery F(nullptr);
	FactoryType('m', F.cost);
}

void OpenFactoryScreen::FactoryTypeCement(){
	CementFactory F(nullptr);
	FactoryType('n', F.cost);
}

void OpenFactoryScreen::FactoryTypeMerchantShip(){
	Shipyard F(nullptr);
	FactoryType('o', F.cost);
}

void OpenFactoryScreen::FactoryTypeSmallArms(){
	SmallArmsFactory F(nullptr);
	FactoryType('p', F.cost);
}

void OpenFactoryScreen::FactoryTypeAmmunition(){
	AmmunitionFactory F(nullptr);
	FactoryType('q', F.cost);
}

void OpenFactoryScreen::FactoryTypeArtillery(){
	ArtilleryFactory F(nullptr);
	FactoryType('r', F.cost);
}

void OpenFactoryScreen::FactoryTypeExplosives(){
	ExplosivesFactory F(nullptr);
	FactoryType('s', F.cost);
}

void OpenFactoryScreen::FactoryTypeTank(){
	TankFactory F(nullptr);
	FactoryType('t', F.cost);
}

void OpenFactoryScreen::FactoryTypeAirship(){
	AirshipFactory F(nullptr);
	FactoryType('u', F.cost);
}

void OpenFactoryScreen::FactoryTypePlane(){
	PlaneFactory F(nullptr);
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
			NF = new LumberMill(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'b':
			NF = new GlassFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'c':
			NF = new CanningFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'd':
			NF = new ClothesFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'e':
			NF = new LiquorDistillery(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'f':
			NF = new FurnitureFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'g':
			NF = new AutomobileFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'h':
			NF = new PaperMill(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'i':
			NF = new TelephoneFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'j':
			NF = new RadioFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'k':
			NF = new MachinePartFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'l':
			NF = new ElectricGearFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'm':
			NF = new FuelRefinery(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'n':
			NF = new CementFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'o':
			NF = new Shipyard(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'p':
			NF = new SmallArmsFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'q':
			NF = new AmmunitionFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'r':
			NF = new ArtilleryFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 's':
			NF = new ExplosivesFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 't':
			NF = new TankFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'u':
			NF = new AirshipFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		case 'v':
			NF = new PlaneFactory(&PCref->CountriesArr[PCref->player_index]->Stock);
			break;
		default:
			return;
			break;
	}

	if (PCref->CountriesArr[PCref->player_index]->Stock.Money >= NF->cost) {
		PCref->StatesArr[index]->AddFactory(NF);
		PCref->CountriesArr[PCref->player_index]->Stock.Money -= NF->cost;
		
		QuitFunc();
	}
}

void OpenFactoryScreen::Close(){
	QuitFunc();
}
