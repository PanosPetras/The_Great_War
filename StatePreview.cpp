#include "ScreenList.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"

#include <cstddef>

namespace {
/*Backgrounds/StatePreview.png is painted at 384x486 and hung in the bottom
left corner of a 1920x1080 window, so a pixel on the parchment is a pixel on
that window. The numbers below are in those pixels and are scaled to whatever
the window is, which keeps them on the icons they belong to.*/
constexpr int ArtWidth = 1920;
constexpr int ArtHeight = 1080;

// Where the top of the parchment lands, 0.55 of the way down the window
constexpr int PanelTop = 594;

/*The raw goods a state produces, painted as three columns of icons with the
amount beside each. The order is the order of the enum, so a raw good added to
the leading run of Good only needs its icon painted into the next free slot.*/
constexpr int GridLeft = 16;
constexpr int GridTop = 54;
constexpr int GridColumns = 3;
constexpr int ColumnPitch = 120;
constexpr int RowPitch = 54;
constexpr int IconSize = 48;
constexpr int NumberGap = 10;

// The population is counted under the grid, in line with its first column
constexpr int PopulationTop = 270;
} // namespace

StatePreview::StatePreview(MainWindow& mw, unsigned id, std::string StateName, std::string controller, PlayerController* PC, const std::array<short int, RawGoodCount>& res, int pop, std::string Factories[4],
                           std::function<void()> CloseFunc, std::function<void(std::unique_ptr<Screen>, std::string)> changeScreenFunc) : Screen(mw), ChangeScreenFunc2(changeScreenFunc), Id(id) {
    auto [Width, Height] = mw.GetWindowDimensions();
    Controller = controller;
    std::string str = "Flags/" + Controller;

    AddLabel<Label>(mw, StateName.c_str(), FontSize::Heading, int(0.04 * Width), int(Height * .55), Width);

    // The number beside every raw good's icon, walking the grid a row at a time
    constexpr int NumberLeft = GridLeft + IconSize + NumberGap;
    for(std::size_t i = 0; i < RawGoodCount; ++i) {
        const int x = NumberLeft + int(i % GridColumns) * ColumnPitch;
        const int y = PanelTop + GridTop + int(i / GridColumns) * RowPitch;
        AddLabel<Label>(mw, std::to_string(res[i]).c_str(), FontSize::Heading, Width * x / ArtWidth, Height * y / ArtHeight);
    }

    AddLabel<Label>(mw, std::to_string(pop).c_str(), FontSize::Heading, Width * NumberLeft / ArtWidth, Height * (PanelTop + PopulationTop) / ArtHeight);

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

    if(Controller == PC->player->GetTag() && Factories[3] == "") {
        AddDrawable<Button>(mw, int(Width * .058), int(Height * 0.95), int(160 * Width / 1920), int(38 * Height / 1080), "Open Factory", FontSize::Caption, [this] { OpenOFS(); });
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
        std::string str = "Icons/Goods/" + std::string(PCref->StatesArr[Id].State_Factories[index]->Kind.name) + ".png";
        AddImage<Image>(*main_window, str, int(Width * (0.055L + 0.0288L * index)), int(Height * 0.8999), 48, 48);
    }
}

void StatePreview::OpenDiplomacyTab() {
    ChangeScreenFunc2(std::make_unique<DiplomacyScreen>(*main_window, PCref, Controller), "DiplomacyScreen");
}
