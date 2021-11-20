#include "ScreenList.h"


StatePreview::StatePreview(SDL_Renderer* r, int Width, int Height, std::string StateName, std::string Controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc) : Screen(r, Width, Height) {
	LabelArr[0] = new Label(r, StateName.c_str(), 32, 0, int(WindowSize[1] * .55));
	LabelArr[1] = new Label(r, std::to_string(res[7]).c_str(), 32, int(Width * 0.03), int(WindowSize[1] * 0.6));
	LabelArr[2] = new Label(r, std::to_string(res[1]).c_str(), 32, int(Width * 0.03), int(WindowSize[1] * 0.65));
	LabelArr[3] = new Label(r, std::to_string(res[2]).c_str(), 32, int(Width * 0.03), int(WindowSize[1] * 0.7));
	LabelArr[4] = new Label(r, std::to_string(res[3]).c_str(), 32, int(Width * 0.03), int(WindowSize[1] * 0.75));
	LabelArr[5] = new Label(r, std::to_string(res[0]).c_str(), 32, int(Width * 0.13), int(WindowSize[1] * 0.6));
	LabelArr[6] = new Label(r, std::to_string(res[4]).c_str(), 32, int(Width * 0.13), int(WindowSize[1] * 0.65));
	LabelArr[7] = new Label(r, std::to_string(res[5]).c_str(), 32, int(Width * 0.13), int(WindowSize[1] * 0.7));
	LabelArr[8] = new Label(r, std::to_string(res[6]).c_str(), 32, int(Width * 0.13), int(WindowSize[1] * 0.75));
	LabelArr[9] = new Label(r, std::to_string(pop).c_str(), 32, int(Width * 0.03), int(WindowSize[1] * 0.8));
	LabelArrtop = 10;

	ImageArr[0] = new Image(r, "Backgrounds/StatePreview.png", 0, int(Height * .55), int(Width * 0.2), int(Height * 0.45));
	std::string str = "Flags/" + Controller + ".png";
	ImageArr[1] = new Image(r, str.c_str(), int(WindowSize[0] * 0.2 - 72 * WindowSize[0] / 1920), int(WindowSize[1] * .55), int(72 * WindowSize[0] / 1920), int(48 * WindowSize[1] / 1080));
	ImageArrtop = 2;

	for (int x = 0; x < 4; x++) {
		if (Factories[x] != "") {
			str = "Icons/Goods/" + Factories[x] + ".png";
			ImageArr[x + 2] = new Image(r, str.c_str(), int(Width * (0.055 + 0.0288 * x)), int(Height * 0.8999), 48, 48);
			ImageArrtop++;
		}
	}

	ButtonArr[0] = new Button(r, int(Width*.2) - int((32 * WindowSize[0] / 1920) / 2), int(Height * .55) - int((32 * WindowSize[1] / 1080) / 2), int(32 * WindowSize[0] /1920), int(32 * WindowSize[1] / 1080), "Buttons/UI/Close", CloseFunc);
	//ButtonArr[1] = new Button();
	ButtonArrtop = 1;
	SliderArrtop = 0;

	PCref = PC;
}