#include "ScreenList.h"

PauseMenu::PauseMenu(SDL_Renderer* r, int Width, int Height, std::function<void()> fp, std::function<void()> UnpauseF, std::function<void(Screen*)> fpl) {
	this->renderer = r;
	bHasBackground = false;
	WindowSize[0] = Height;
	WindowSize[1] = Width;
	auto change = std::bind(&PauseMenu::ReturnToMainMenu, this);
	ImageArr[0] = new Image(renderer, "Backgrounds/Pause_Menu.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	ButtonArr[0] = new Button(r, int(Width * 0.36), int(Height * 0.35), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Back", UnpauseF);
	ButtonArr[1] = new Button(r, int(Width * 0.342), int(Height * 0.45), int(Width * 0.12), int(Height * 0.06), "Buttons/Menus/Save_Game");
	ButtonArr[2] = new Button(r, int(Width * 0.355), int(Height * 0.55), int(Width * 0.09), int(Height * 0.06), "Buttons/Menus/Resign", change);
	ButtonArr[3] = new Button(r, int(Width * 0.36), int(Height * 0.65), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Quit", fp);
	ButtonArrtop = 4;
	LabelArrtop = 0;
	ImageArrtop = 1;
	bHasBackground = false;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void PauseMenu::ReturnToMainMenu(){
	MainMenu* MM = new MainMenu(renderer, WindowSize[1], WindowSize[0], QuitFunc, ChangeScreenFunc);
	ChangeScreenFunc(MM);
}