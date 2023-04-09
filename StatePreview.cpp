#include "PlayerController.h"
#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

StatePreview::StatePreview(SDL_Renderer* r, int id, std::string StateName, std::string controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc, std::function<void(Screen*, std::string)> ChangeScreenFunc) : Screen(r) {
	int Width = GetWindowWidth(), Height = GetWindowHeight();
	Controller = controller;
	std::string str = "Flags/" + Controller;

	AddLabel<Label>(r, StateName.c_str(), 32, int(0.04 * Width), int(Height * .55), Width);
	AddLabel<Label>(r, std::to_string(res[7]).c_str(), 32, int(Width * 0.03), int(Height * 0.6));
	AddLabel<Label>(r, std::to_string(res[1]).c_str(), 32, int(Width * 0.03), int(Height * 0.65));
	AddLabel<Label>(r, std::to_string(res[2]).c_str(), 32, int(Width * 0.03), int(Height * 0.7));
	AddLabel<Label>(r, std::to_string(res[3]).c_str(), 32, int(Width * 0.03), int(Height * 0.75));
	AddLabel<Label>(r, std::to_string(res[0]).c_str(), 32, int(Width * 0.13), int(Height * 0.6));
	AddLabel<Label>(r, std::to_string(res[4]).c_str(), 32, int(Width * 0.13), int(Height * 0.65));
	AddLabel<Label>(r, std::to_string(res[5]).c_str(), 32, int(Width * 0.13), int(Height * 0.7));
	AddLabel<Label>(r, std::to_string(res[6]).c_str(), 32, int(Width * 0.13), int(Height * 0.75));
	AddLabel<Label>(r, std::to_string(pop).c_str(), 32, int(Width * 0.03), int(Height * 0.8));

	AddImage<Image>(r, "Backgrounds/StatePreview.png", 0, int(Height * .55), int(Width * 0.2), int(Height * 0.45));

	AddDrawable<Button>(r, 0, int(Height * .55), int(72 * Width / 1920), int(48 * Height / 1080), str, [this]{ OpenDiplomacyTab();});
	AddDrawable<Button>(r, int(Width * .2) - int((32 * Width / 1920) / 2), int(Height * .55) - int((32 * Height / 1080) / 2), int(32 * Width / 1920), int(32 * Height / 1080), "Buttons/UI/Close", CloseFunc);

	for (int x = 0; x < 4; x++) {
		if (Factories[x] != "") {
			str = "Icons/Goods/" + Factories[x] + ".png";
			AddImage<Image>(r, str.c_str(), int(Width * (0.055 + 0.0288 * x)), int(Height * 0.8999), 48, 48);
		} else {
			break;
		}
	}

	if (Controller == PC->player_tag && Factories[3] == "") {
		AddDrawable<Button>(r, int(Width * .058), int(Height * 0.95), int(160 * Width / 1920), int(38 * Height / 1080), "Open Factory", 24, [this]{OpenOFS();});
	}

	PCref = PC;
	this->Id = id;
	ChangeScreenFunc2 = ChangeScreenFunc;
}

StatePreview::~StatePreview() {
        delete OFS;
}

void StatePreview::Render(){
	//Calls the method responsible for rendering the background
	if (bHasBackground) {
		this->RenderBackground();
	}

	//Calls the render method for every active image
	for (auto& image : ImageArr) {
		image->Draw();
	}

	//Calls the render method for every active button
	for (auto& drawable : InputDrawableArr) {
		drawable->Draw();
	}

	//Calls the render method for every active label
	for (auto& label : LabelArr) {
		label->Draw();
	}

	if (OFS != nullptr) {
		OFS->Render();
	}
}

void StatePreview::Handle_Input(SDL_Event* ev){
	if (OFS != nullptr) {
		OFS->Handle_Input(ev);
	}
	for (auto& drawable : InputDrawableArr) {
		drawable->HandleInput(ev);
	}
}

void StatePreview::OpenOFS(){
	if (OFS == nullptr) {
		auto del = std::bind(&StatePreview::DeleteOFS, this);
		OFS = new OpenFactoryScreen(renderer, Id, PCref, del);
	}
}

void StatePreview::DeleteOFS(){
	delete OFS;
	OFS = nullptr;

	int Width = GetWindowWidth(), Height = GetWindowHeight();

	if (PCref->StatesArr[Id]->State_Factories[3] != nullptr) {
                InputDrawableArr.resize(InputDrawableArrtop()-1);
	}
	for (int x = ImageArrtop() - 1; x < 4; x++) {
		if (PCref->StatesArr[Id]->State_Factories[x] != nullptr) {
			std::string str = "Icons/Goods/" + PCref->StatesArr[Id]->State_Factories[x]->Type + ".png";
			AddImage<Image>(renderer, str, int(Width * (0.055 + 0.0288 * x)), int(Height * 0.8999), 48, 48);
		} else {
			break;
		}
	}
}

void StatePreview::OpenDiplomacyTab() {
	Screen* NS = new DiplomacyScreen(renderer, PCref, Controller);
	ChangeScreenFunc2(NS, "DiplomacyScreen");
}
