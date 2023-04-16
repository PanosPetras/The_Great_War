#include "ScreenList.h"

#include "MainWindow.h"
#include "Label.h"
#include "Button.h"

IndustryScreen::IndustryScreen(MainWindow& mw, int Stockpile[30]) : Screen(mw) {
	SetupBg("Backgrounds/Industry1.png");
        auto [Width, Height] = mw.GetWindowDimensions();

	AddLabel<Label>(mw, std::to_string(Stockpile[0]), 32, int(Width * 0.24), int(Height * 0.165));
	AddLabel<Label>(mw, std::to_string(Stockpile[5]), 32, int(Width * 0.365), int(Height * 0.165));
	AddLabel<Label>(mw, std::to_string(Stockpile[2]), 32, int(Width * 0.49), int(Height * 0.165));
	AddLabel<Label>(mw, std::to_string(Stockpile[4]), 32, int(Width * 0.615), int(Height * 0.165));
	AddLabel<Label>(mw, std::to_string(Stockpile[3]), 32, int(Width * 0.24), int(Height * 0.26));
	AddLabel<Label>(mw, std::to_string(Stockpile[1]), 32, int(Width * 0.365), int(Height * 0.26));
	AddLabel<Label>(mw, std::to_string(Stockpile[11]), 32, int(Width * 0.49), int(Height * 0.26));
	AddLabel<Label>(mw, std::to_string(Stockpile[10]), 32, int(Width * 0.615), int(Height * 0.26));
	AddLabel<Label>(mw, std::to_string(Stockpile[6]), 32, int(Width * 0.74), int(Height * 0.21));
	AddLabel<Label>(mw, std::to_string(Stockpile[19]), 32, int(Width * 0.24), int(Height * 0.395));
	AddLabel<Label>(mw, std::to_string(Stockpile[21]), 32, int(Width * 0.365), int(Height * 0.395));
	AddLabel<Label>(mw, std::to_string(Stockpile[28]), 32, int(Width * 0.49), int(Height * 0.395));
	AddLabel<Label>(mw, std::to_string(Stockpile[20]), 32, int(Width * 0.615), int(Height * 0.395));
	AddLabel<Label>(mw, std::to_string(Stockpile[27]), 32, int(Width * 0.24), int(Height * 0.49));
	AddLabel<Label>(mw, std::to_string(Stockpile[25]), 32, int(Width * 0.365), int(Height * 0.49));
	AddLabel<Label>(mw, std::to_string(Stockpile[7]), 32, int(Width * 0.49), int(Height * 0.49));
	AddLabel<Label>(mw, std::to_string(Stockpile[24]), 32, int(Width * 0.615), int(Height * 0.49));
	AddLabel<Label>(mw, std::to_string(Stockpile[22]), 32, int(Width * 0.74), int(Height * 0.44));
	AddLabel<Label>(mw, std::to_string(Stockpile[9]), 32, int(Width * 0.24), int(Height * 0.63));
	AddLabel<Label>(mw, std::to_string(Stockpile[8]), 32, int(Width * 0.365), int(Height * 0.63));
	AddLabel<Label>(mw, std::to_string(Stockpile[26]), 32, int(Width * 0.49), int(Height * 0.63));
	AddLabel<Label>(mw, std::to_string(Stockpile[12]), 32, int(Width * 0.615), int(Height * 0.63));
	AddLabel<Label>(mw, std::to_string(Stockpile[23]), 32, int(Width * 0.74), int(Height * 0.63));
	AddLabel<Label>(mw, std::to_string(Stockpile[16]), 32, int(Width * 0.24), int(Height * 0.765));
	AddLabel<Label>(mw, std::to_string(Stockpile[13]), 32, int(Width * 0.365), int(Height * 0.765));
	AddLabel<Label>(mw, std::to_string(Stockpile[17]), 32, int(Width * 0.49), int(Height * 0.765));
	AddLabel<Label>(mw, std::to_string(Stockpile[15]), 32, int(Width * 0.615), int(Height * 0.765));
	AddLabel<Label>(mw, std::to_string(Stockpile[18]), 32, int(Width * 0.74), int(Height * 0.765));
	AddLabel<Label>(mw, std::to_string(Stockpile[14]), 32, int(Width * 0.49), int(Height * 0.86));
	AddLabel<Label>(mw, std::to_string(Stockpile[29]), 32, int(Width * 0.365), int(Height * 0.86));
}

void IndustryScreen::UpdateText(int Stockpile[29]){
	LabelArr[0]->ChangeText(std::to_string(Stockpile[0]));
	LabelArr[1]->ChangeText(std::to_string(Stockpile[5]));
	LabelArr[2]->ChangeText(std::to_string(Stockpile[2]));
	LabelArr[3]->ChangeText(std::to_string(Stockpile[4]));
	LabelArr[4]->ChangeText(std::to_string(Stockpile[3]));
	LabelArr[5]->ChangeText(std::to_string(Stockpile[1]));
	LabelArr[6]->ChangeText(std::to_string(Stockpile[11]));
	LabelArr[7]->ChangeText(std::to_string(Stockpile[10]));
	LabelArr[8]->ChangeText(std::to_string(Stockpile[6]));
	LabelArr[9]->ChangeText(std::to_string(Stockpile[19]));
	LabelArr[10]->ChangeText(std::to_string(Stockpile[21]));
	LabelArr[11]->ChangeText(std::to_string(Stockpile[28]));
	LabelArr[12]->ChangeText(std::to_string(Stockpile[20]));
	LabelArr[13]->ChangeText(std::to_string(Stockpile[27]));
	LabelArr[14]->ChangeText(std::to_string(Stockpile[25]));
	LabelArr[15]->ChangeText(std::to_string(Stockpile[7]));
	LabelArr[16]->ChangeText(std::to_string(Stockpile[24]));
	LabelArr[17]->ChangeText(std::to_string(Stockpile[22]));
	LabelArr[18]->ChangeText(std::to_string(Stockpile[9]));
	LabelArr[19]->ChangeText(std::to_string(Stockpile[8]));
	LabelArr[20]->ChangeText(std::to_string(Stockpile[26]));
	LabelArr[21]->ChangeText(std::to_string(Stockpile[12]));
	LabelArr[22]->ChangeText(std::to_string(Stockpile[23]));
	LabelArr[23]->ChangeText(std::to_string(Stockpile[16]));
	LabelArr[24]->ChangeText(std::to_string(Stockpile[13]));
	LabelArr[25]->ChangeText(std::to_string(Stockpile[17]));
	LabelArr[26]->ChangeText(std::to_string(Stockpile[15]));
	LabelArr[27]->ChangeText(std::to_string(Stockpile[18]));
	LabelArr[28]->ChangeText(std::to_string(Stockpile[14]));
	LabelArr[29]->ChangeText(std::to_string(Stockpile[29]));
}
