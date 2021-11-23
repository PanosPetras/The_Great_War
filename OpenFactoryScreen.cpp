#include "ScreenList.h"


OpenFactoryScreen::OpenFactoryScreen(SDL_Renderer* r, int Width, int Height, std::function<void()> fp = NULL, std::function<void(Screen*)> fpl = NULL) : Screen(r, Width, Height){
	bHasBackground = false;
	ImageArr[0] = new Image(renderer, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
	ButtonArr[0] = new Button(r, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Buttons/Menus/Back");
	ButtonArr[1] = new Button(r, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Buttons/Menus/Confirm");
	ButtonArr[2] = new Button(r, int(Width * 0.3), int(Height * 0.25), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/lumber");
	ButtonArr[3] = new Button(r, int(Width * 0.35), int(Height * 0.25), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/glass");
	ButtonArr[4] = new Button(r, int(Width * 0.4), int(Height * 0.25), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/canned food");
	ButtonArrtop = 5;
	LabelArrtop = 0;
	ImageArrtop = 1;
	SliderArrtop = 0;
	bHasBackground = false;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}