#include "ScreenList.h"

StatePreview::StatePreview(SDL_Renderer* r, int Width, int Height, int id, std::string StateName, std::string Controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc) : Screen(r, Width, Height) {
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
		} else {
			break;
		}
	}

	ButtonArr[0] = new Button(r, int(Width*.2) - int((32 * WindowSize[0] / 1920) / 2), int(Height * .55) - int((32 * WindowSize[1] / 1080) / 2), int(32 * WindowSize[0] /1920), int(32 * WindowSize[1] / 1080), "Buttons/UI/Close", CloseFunc);
	ButtonArrtop = 1;
	if (Controller == PC->player_tag && Factories[3] == "") {
		auto change = std::bind(&StatePreview::OpenOFS, this);
		ButtonArr[1] = new Button(r, int(Width * .058), int(Height * 0.95), int(160 * WindowSize[0] / 1920), int(38 * WindowSize[1] / 1080), "Buttons/UI/Open_Factory", change);
		ButtonArrtop++;
	}
	SliderArrtop = 0;

	PCref = PC;
	this->Id = id;
}

StatePreview::~StatePreview(){
	if (OFS != nullptr) {
		delete OFS;
	}
}

void StatePreview::Render(){
	//Calls the method responsible for rendering the background
	if (bHasBackground) {
		this->RenderBackground();
	}

	//Calls the render method for every active image
	for (int x = 0; x < ImageArrtop; x++) {
		ImageArr[x]->RenderImage();
	}

	//Calls the render method for every active slider
	for (int x = 0; x < SliderArrtop; x++) {
		SliderArr[x]->RenderSlider();
	}

	//Calls the render method for every active button
	for (int x = 0; x < ButtonArrtop; x++) {
		ButtonArr[x]->RenderButton();
	}

	//Calls the render method for every active label
	for (int x = 0; x < LabelArrtop; x++) {
		LabelArr[x]->RenderLabel();
	}

	if (OFS != nullptr) {
		OFS->Render();
	}
}

void StatePreview::Handle_Input(SDL_Event* ev){
	if (OFS != nullptr) {
		OFS->Handle_Input(ev);
	}
	for (int x = 0; x < ButtonArrtop; x++) {
		ButtonArr[x]->button_process_event(ev);
	}
}

void StatePreview::OpenOFS(){
	if (OFS == nullptr) {
		auto del = std::bind(&StatePreview::DeleteOFS, this);
		OFS = new OpenFactoryScreen(renderer, WindowSize[0], WindowSize[1], Id, PCref, del);
	}
}

void StatePreview::DeleteOFS(){
	delete OFS;
	OFS = nullptr;
	if (PCref->StatesArr[Id]->State_Factories[3] != nullptr) {
		delete ButtonArr[1];
		ButtonArrtop = 1;
	}
	for (int x = ImageArrtop - 2; x < 4; x++) {
		if (PCref->StatesArr[Id]->State_Factories[x] != nullptr) {
			std::string str = "Icons/Goods/" + PCref->StatesArr[Id]->State_Factories[x]->Type + ".png";
			ImageArr[x + 2] = new Image(renderer, str.c_str(), int(WindowSize[0] * (0.055 + 0.0288 * x)), int(WindowSize[1] * 0.8999), 48, 48);
			ImageArrtop++;
		}
		else {
			break;
		}
	}
}
