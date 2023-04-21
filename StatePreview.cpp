#include "ScreenList.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"

StatePreview::StatePreview(MainWindow& mw, unsigned id, std::string StateName, std::string controller, PlayerController* PC, int res[8], int pop, std::string Factories[4], std::function<void()> CloseFunc,
                           std::function<void(std::unique_ptr<Screen>, std::string)> changeScreenFunc) :
    Screen(mw),
    ChangeScreenFunc2(changeScreenFunc), Id(id) {
    auto [Width, Height] = mw.GetWindowDimensions();
    Controller = controller;
    std::string str = "Flags/" + Controller;

    AddLabel<Label>(mw, StateName.c_str(), 32, int(0.04 * Width), int(Height * .55), Width);
    AddLabel<Label>(mw, std::to_string(res[7]).c_str(), 32, int(Width * 0.03), int(Height * 0.6));
    AddLabel<Label>(mw, std::to_string(res[1]).c_str(), 32, int(Width * 0.03), int(Height * 0.65));
    AddLabel<Label>(mw, std::to_string(res[2]).c_str(), 32, int(Width * 0.03), int(Height * 0.7));
    AddLabel<Label>(mw, std::to_string(res[3]).c_str(), 32, int(Width * 0.03), int(Height * 0.75));
    AddLabel<Label>(mw, std::to_string(res[0]).c_str(), 32, int(Width * 0.13), int(Height * 0.6));
    AddLabel<Label>(mw, std::to_string(res[4]).c_str(), 32, int(Width * 0.13), int(Height * 0.65));
    AddLabel<Label>(mw, std::to_string(res[5]).c_str(), 32, int(Width * 0.13), int(Height * 0.7));
    AddLabel<Label>(mw, std::to_string(res[6]).c_str(), 32, int(Width * 0.13), int(Height * 0.75));
    AddLabel<Label>(mw, std::to_string(pop).c_str(), 32, int(Width * 0.03), int(Height * 0.8));

    AddImage<Image>(mw, "Backgrounds/StatePreview.png", 0, int(Height * .55), int(Width * 0.2), int(Height * 0.45));

    AddDrawable<Button>(mw, 0, int(Height * .55), int(72 * Width / 1920), int(48 * Height / 1080), str, [this] { OpenDiplomacyTab(); });
    AddDrawable<Button>(mw, int(Width * .2) - int((32 * Width / 1920) / 2), int(Height * .55) - int((32 * Height / 1080) / 2), int(32 * Width / 1920), int(32 * Height / 1080), "Buttons/UI/Close", CloseFunc);

    for(int x = 0; x < 4; x++) {
        if(Factories[x] != "") {
            str = "Icons/Goods/" + Factories[x] + ".png";
            AddImage<Image>(mw, str.c_str(), int(Width * (0.055 + 0.0288 * x)), int(Height * 0.8999), 48, 48);
        } else {
            break;
        }
    }

    if(Controller == PC->player_tag && Factories[3] == "") {
        AddDrawable<Button>(mw, int(Width * .058), int(Height * 0.95), int(160 * Width / 1920), int(38 * Height / 1080), "Open Factory", 24, [this] { OpenOFS(); });
    }

    PCref = PC;
}

void StatePreview::Render() {
    // Calls the method responsible for rendering the background
    if(bHasBackground) {
        this->RenderBackground();
    }

    // Calls the render method for every active image
    for(auto& image : ImageArr) {
        image->Draw();
    }

    // Calls the render method for every active button
    for(auto& drawable : InputDrawableArr) {
        drawable->Draw();
    }

    // Calls the render method for every active label
    for(auto& label : LabelArr) {
        label->Draw();
    }

    if(OFS) {
        OFS->Render();
    }
}

void StatePreview::Handle_Input(SDL_Event& ev) {
    if(OFS) {
        OFS->Handle_Input(ev);
    }
    for(auto& drawable : InputDrawableArr) {
        drawable->HandleInput(ev);
    }
}

void StatePreview::OpenOFS() {
    if(!OFS) {
        OFS = std::make_unique<OpenFactoryScreen>(*main_window, Id, PCref, [this] { DeleteOFS(); });
    }
}

void StatePreview::DeleteOFS() {
    OFS.reset();
    auto [Width, Height] = main_window->GetWindowDimensions();

    if(PCref->StatesArr[Id].State_Factories[3] != nullptr) {
        InputDrawableArr.resize(InputDrawableArrtop() - 1);
    }

    if(ImageArr.empty()) throw std::out_of_range("StatePreview::DeleteOFS ImageArr is empty");
    auto index = ImageArr.size() - 1;
    if(PCref->StatesArr[Id].State_Factories[index] != nullptr) {
        std::string str = "Icons/Goods/" + PCref->StatesArr[Id].State_Factories[index]->Type + ".png";
        AddImage<Image>(*main_window, str, int(Width * (0.055L + 0.0288L * index)), int(Height * 0.8999), 48, 48);
    }
}

void StatePreview::OpenDiplomacyTab() {
    ChangeScreenFunc2(std::make_unique<DiplomacyScreen>(*main_window, PCref, Controller), "DiplomacyScreen");
}
