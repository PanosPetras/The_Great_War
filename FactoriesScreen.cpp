#include "ScreenList.h"


FactoriesScreen::FactoriesScreen(SDL_Renderer* r, int Width, int Height, std::vector<std::string> StatesNames, std::vector<std::vector<std::string>> Factories, std::function<void()> fp, std::function<void(Screen*)> fpl) {
	renderer = r;
	bHasBackground = true;
	SetupBg("Backgrounds/FactoriesScreen.png");
	WindowSize[0] = Width;
	WindowSize[1] = Height;

	LabelArrtop = 0;
	ImageArrtop = 0;
	std::string path;
	for (int x = 0; x < 6; x++){
		if (StatesNames[x] != "") {
			LabelArr[x] = new Label(r, StatesNames[x].c_str(), 32, 420 + (x % 2) * 650, 140 + int(x / 2) * 300);
			LabelArrtop++;

			for (int i = 0; i < 4; i++) {
				if (Factories[x][i] != "") {
					path = "Icons/Goods/" + Factories[x][i] + ".png";
					ImageArr[ImageArrtop] = new Image(r, path.c_str(), 447 + (x % 2) * 650, 285 + int(x / 2) * 300, 36, 36);
					ImageArrtop++;
				}
			}
		} else {
			break;
		}
	}
	StateNames = StatesNames;
	StateFactories = Factories;

	ButtonArrtop = 0;

	SliderArr[0] = new Slider(r, int(Width * 0.5), int(Height * 0.5), 150, 25, 0, 100);
	SliderArrtop = 1;

	index = 0;
}

void FactoriesScreen::UpdateFactoriesStats(int Value){
	LabelArrtop = 0;
	ImageArrtop = 0;
	std::string path;
	index = Value;
	for (int x = 0; x < 6; x++) {
		if (StateNames[x + index * 2] != "") {
			LabelArr[x] = new Label(renderer, StateNames[x + index * 2].c_str(), 32, 420 + (x % 2) * 650, 140 + int(x / 2) * 300);
			LabelArrtop++;

			for (int i = 0; i < 4; i++) {
				if (StateFactories[x + index * 2][i] != "") {
					path = "Icons/Goods/" + StateFactories[x + index * 2][i] + ".png";
					ImageArr[ImageArrtop] = new Image(renderer, path.c_str(), 447 + (x % 2) * 650, 285 + int(x / 2) * 300, 36, 36);
					ImageArrtop++;
				}
			}
		}
		else {
			break;
		}
	}
}