#include "ScreenList.h"
#include "WindowInfo.h"

CountrySelection::CountrySelection(SDL_Renderer* r, std::function<void()> UnpauseF, std::function<void(Screen*)> fpl) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/CountrySelection.png");

	int fontSize = 32, Width = GetWindowWidth(), Height = GetWindowHeight();
	auto change = std::bind(&CountrySelection::StartGame, this);

	InputDrawableArr[0] = new Button(r, int(Width * 0.85), int(Height * 0.85), int(Width * 0.09), int(Height * 0.06), "Confirm", 32, change, SDLK_KP_ENTER);
	change = std::bind(&CountrySelection::Back, this);
	InputDrawableArr[1] = new Button(r, int(Width * 0.1), int(Height * 0.85), int(Width * 0.08), int(Height * 0.06), "Back", 32, change, SDLK_ESCAPE);

	change = std::bind(&CountrySelection::SelectGER, this);
	InputDrawableArr[2] = new Button(r, int(Width * 0.3), int(Height * 0.2), int(Width * 0.06), int(Height * 0.06), "Flags/ger", change);
	change = std::bind(&CountrySelection::SelectFRA, this);
	InputDrawableArr[3] = new Button(r, int(Width * 0.3), int(Height * 0.3), int(Width * 0.06), int(Height * 0.06), "Flags/fra", change);
	change = std::bind(&CountrySelection::SelectENG, this);
	InputDrawableArr[4] = new Button(r, int(Width * 0.3), int(Height * 0.4), int(Width * 0.06), int(Height * 0.06), "Flags/eng", change);
	change = std::bind(&CountrySelection::SelectKUK, this);
	InputDrawableArr[5] = new Button(r, int(Width * 0.3), int(Height * 0.5), int(Width * 0.06), int(Height * 0.06), "Flags/kuk", change);
	change = std::bind(&CountrySelection::SelectITA, this);
	InputDrawableArr[6] = new Button(r, int(Width * 0.635), int(Height * 0.3), int(Width * 0.06), int(Height * 0.06), "Flags/ita", change);
	change = std::bind(&CountrySelection::SelectRUS, this);
	InputDrawableArr[7] = new Button(r, int(Width * 0.635), int(Height * 0.4), int(Width * 0.06), int(Height * 0.06), "Flags/rus", change);
	change = std::bind(&CountrySelection::SelectOTT, this);
	InputDrawableArr[8] = new Button(r, int(Width * 0.635), int(Height * 0.5), int(Width * 0.06), int(Height * 0.06), "Flags/ott", change);
	change = std::bind(&CountrySelection::SelectUSA, this);
	InputDrawableArr[9] = new Button(r, int(Width * 0.635), int(Height * 0.2), int(Width * 0.06), int(Height * 0.06), "Flags/usa", change);

	LabelArr[0] = new Label(r, "The World in 1910", int(Height * 0.03), int(Width * 0.43), int(Height * 0.07));
	LabelArr[1] = new Label(r, "Choose your nation", int(Height * 0.03), int(Width * 0.427), int(Height * 0.13));
	LabelArr[2] = new Label(r, "German Empire", int(Height * 0.023), int(Width * 0.37), int(Height * 0.21));
	LabelArr[3] = new Label(r, "French Republic", int(Height * 0.023), int(Width * 0.37), int(Height * 0.31));
	LabelArr[4] = new Label(r, "British Empire", int(Height * 0.023), int(Width * 0.37), int(Height * 0.41));
	LabelArr[5] = new Label(r, "Austria-Hungary", int(Height * 0.023), int(Width * 0.37), int(Height * 0.51));
	LabelArr[6] = new Label(r, "United States", int(Height * 0.023), int(Width * 0.55), int(Height * 0.21));
	LabelArr[7] = new Label(r, "Kingdom of Italy", int(Height * 0.023), int(Width * 0.53), int(Height * 0.31));
	LabelArr[8] = new Label(r, "Russian Empire", int(Height * 0.023), int(Width * 0.538), int(Height * 0.41));
	LabelArr[9] = new Label(r, "Ottoman Empire", int(Height * 0.023), int(Width * 0.53), int(Height * 0.51));

	ImageArr[0] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.428), int(Height * 0.07), int(Width * 0.14), int(Height * 0.04));
	ImageArr[1] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.425), int(Height * 0.13), int(Width * 0.146), int(Height * 0.04));
	ImageArr[2] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.195), int(Width * 0.069), int(Height * 0.072));
	ImageArr[3] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.295), int(Width * 0.069), int(Height * 0.072));
	ImageArr[4] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.395), int(Width * 0.069), int(Height * 0.072));
	ImageArr[5] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.296), int(Height * 0.495), int(Width * 0.069), int(Height * 0.072));
	ImageArr[6] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.195), int(Width * 0.069), int(Height * 0.072));
	ImageArr[7] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.295), int(Width * 0.069), int(Height * 0.072));
	ImageArr[8] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.395), int(Width * 0.069), int(Height * 0.072));
	ImageArr[9] = new Image(r, "Backgrounds/FlagBg.png", int(Width * 0.631), int(Height * 0.495), int(Width * 0.069), int(Height * 0.072));

	ImageArr[10] = new Image(r, "Icons/right.png", -Width, -Height, 64 * Width / 1920, 64 * Height / 1080);

	InputDrawableArrtop = 10;
	LabelArrtop = 10;
	ImageArrtop = 11;

	ChangeScreenFunc = fpl;
	mousepressed = false;
	ChangeScreenFunc = fpl;
	QuitFunc = UnpauseF;

	tags[0] = "ger";
	tags[1] = "fra";
	tags[2] = "eng";
	tags[3] = "kuk";
	tags[4] = "ita";
	tags[5] = "rus";
	tags[6] = "ott";
	tags[7] = "usa";

	colors[0][0], colors[0][1], colors[0][2] = 42, 42, 42;
	colors[1][0], colors[1][1], colors[1][2] = 18, 30, 152; 
	colors[2][0], colors[2][1], colors[2][2] = 128, 0, 32;
	colors[3][0], colors[3][1], colors[3][2] = 255, 255, 255;
	colors[4][0], colors[4][1], colors[4][2] = 38, 160, 64;
	colors[5][0], colors[5][1], colors[5][2] = 64, 160, 96;
	colors[6][0], colors[6][1], colors[6][2] = 160, 144, 96;
	colors[7][0], colors[7][1], colors[7][2] = 0, 96, 128;

	CountryIndex = -1;
}

void CountrySelection::Back(){
	MainMenu* MM = new MainMenu(renderer, QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(MM);
}

void CountrySelection::SelectGER() {
	CountryIndex = 0;
	ChangeArrow(int(GetWindowWidth() * 0.262), int(GetWindowHeight() * 0.202), 1);
}

void CountrySelection::SelectENG() {
	CountryIndex = 2;
	ChangeArrow(int(GetWindowWidth() * 0.262), int(GetWindowHeight() * 0.402), 1);
}

void CountrySelection::SelectFRA() {
	CountryIndex = 1;
	ChangeArrow(int(GetWindowWidth() * 0.262), int(GetWindowHeight() * 0.302), 1);
}

void CountrySelection::SelectKUK() {
	CountryIndex = 3;
	ChangeArrow(int(GetWindowWidth() * 0.262), int(GetWindowHeight() * 0.502), 1);
}

void CountrySelection::SelectITA() {
	CountryIndex = 4;
	ChangeArrow(int(GetWindowWidth() * 0.7), int(GetWindowHeight() * 0.302), 2);
}

void CountrySelection::SelectRUS() {
	CountryIndex = 5;
	ChangeArrow(int(GetWindowWidth() * 0.7), int(GetWindowHeight() * 0.402), 2);
}

void CountrySelection::SelectOTT() {
	CountryIndex = 6;
	ChangeArrow(int(GetWindowWidth() * 0.7), int(GetWindowHeight() * 0.502), 2);
}

void CountrySelection::SelectUSA() {
	CountryIndex = 7;
	ChangeArrow(int(GetWindowWidth() * 0.7), int(GetWindowHeight() * 0.202), 2);
}

void CountrySelection::ChangeArrow(int x, int y, int img) {
	ImageArr[10]->ChangePosition(x, y, int(64 * GetWindowWidth() / 1920), int(64 * GetWindowHeight() / 1080));
	if (img == 1) {
		ImageArr[10]->ChangeImage("Icons/right.png");
	} else if (img == 2) {
		ImageArr[10]->ChangeImage("Icons/left.png");
	}
}

void CountrySelection::StartGame() {
	if (CountryIndex != -1) {
		GameScreen* GS = new GameScreen(renderer, tags[CountryIndex], QuitFunc, ChangeScreenFunc);
		ChangeScreenFunc(GS);
	}
}