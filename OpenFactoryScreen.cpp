#include "ScreenList.h"

#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"

#include <cstddef>
#include <iterator>
#include <string>

namespace {
/*The grid the factory icons sit on, as fractions of the window's dimensions.
The kinds fill it left to right, a row at a time.*/
constexpr double ButtonColumns[]{0.3, 0.35, 0.4, 0.45, 0.5};
constexpr double ButtonRows[]{0.3, 0.36, 0.42, 0.48, 0.54, 0.6};

/*The kinds the screen offers, which are the leading run of the factory table.
The synthetic refineries sit past the end: they are the way round a blockade
rather than something to build on day one, so they wait on Technology.*/
constexpr std::size_t OfferedKinds = std::size_t(FactoryType::SyntheticRubberRefinery);
static_assert(OfferedKinds <= std::size(ButtonColumns) * std::size(ButtonRows));
} // namespace

OpenFactoryScreen::OpenFactoryScreen(MainWindow& mw, unsigned id, PlayerController* PC, std::function<void()> quitfunc) : Screen(mw, quitfunc) {
    PCref = PC;
    auto [Width, Height] = mw.GetWindowDimensions();

    AddImage<Image>(mw, "Backgrounds/factory1.png", int(Width * 0.25), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));
    AddLabel<Label>(mw, "Open Factory", FontSize::Heading, int(Width * 0.45), int(Height * 0.22));
    std::string lbl1txt = "Current Funds: " + std::to_string(PC->player->Stock.Money);
    AddLabel<Label>(mw, lbl1txt.c_str(), FontSize::Heading, int(Width * 0.55), int(Height * 0.35));
    lbl1txt = "Factory cost: " + std::to_string(10);
    AddLabel<Label>(mw, lbl1txt.c_str(), FontSize::Heading, int(Width * 0.55), int(Height * 0.41));

    AddDrawable<Button>(mw, int(Width * 0.32), int(Height * 0.7), int(Width * 0.08), int(Height * 0.06), "Back", FontSize::Heading, [this] { Close(); });
    AddDrawable<Button>(mw, int(Width * 0.59), int(Height * 0.7), int(Width * 0.1), int(Height * 0.06), "Confirm", FontSize::Heading, [this] { BuildFactory(); });

    // One icon per kind on offer, each one picking the kind it stands for
    for(std::size_t i = 0; i < OfferedKinds; i++) {
        const FactoryKind& kind = FactoryKinds[i];
        const double x = ButtonColumns[i % std::size(ButtonColumns)];
        const double y = ButtonRows[i / std::size(ButtonColumns)];

        AddDrawable<Button>(mw, int(Width * x), int(Height * y), int(Width * 0.025), int(Height * 0.0444), "Icons/Goods/" + std::string(kind.name), [this, type = kind.type] { SelectFactory(type); });
    }

    index = id;
}

void OpenFactoryScreen::SelectFactory(FactoryType kind) {
    selected = kind;

    std::string txt = "Factory cost: " + std::to_string(KindOf(kind).cost);
    LabelArr[2]->ChangeText(txt.c_str());
}

void OpenFactoryScreen::BuildFactory() {
    if(!selected) return;

    const FactoryKind& kind = KindOf(*selected);
    if(PCref->player->Stock.Money < kind.cost) return;

    auto NF = std::make_unique<Factory>(*selected, &PCref->player->Stock, &PCref->WorldMarket);

    PCref->player->Stock.Money -= kind.cost;
    PCref->StatesArr[index].AddFactory(NF);

    QuitFunc();
}

void OpenFactoryScreen::Close() {
    QuitFunc();
}
