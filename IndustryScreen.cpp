#include "ScreenList.h"

IndustryScreen::IndustryScreen(SDL_Renderer* r, int Width, int Height, int Stockpile[30], std::function<void()> fp, std::function<void(Screen*)> fpl){
	this->renderer = r;
	bHasBackground = true;
	SetupBg("Backgrounds/Industry1.png");
	WindowSize[0] = Height;
	WindowSize[1] = Width;
	ButtonArrtop = 0;
	LabelArr[0] = new Label(r, std::to_string(Stockpile[0]).c_str(), 32, int(Width * 0.24), int(Height * 0.165));
	LabelArr[1] = new Label(r, std::to_string(Stockpile[5]).c_str(), 32, int(Width * 0.365), int(Height * 0.165));
	LabelArr[2] = new Label(r, std::to_string(Stockpile[2]).c_str(), 32, int(Width * 0.49), int(Height * 0.165));
	LabelArr[3] = new Label(r, std::to_string(Stockpile[4]).c_str(), 32, int(Width * 0.615), int(Height * 0.165));
	LabelArr[4] = new Label(r, std::to_string(Stockpile[3]).c_str(), 32, int(Width * 0.24), int(Height * 0.26));
	LabelArr[5] = new Label(r, std::to_string(Stockpile[1]).c_str(), 32, int(Width * 0.365), int(Height * 0.26));
	LabelArr[6] = new Label(r, std::to_string(Stockpile[11]).c_str(), 32, int(Width * 0.49), int(Height * 0.26));
	LabelArr[7] = new Label(r, std::to_string(Stockpile[10]).c_str(), 32, int(Width * 0.615), int(Height * 0.26));
	LabelArr[8] = new Label(r, std::to_string(Stockpile[6]).c_str(), 32, int(Width * 0.74), int(Height * 0.21));
	LabelArr[9] = new Label(r, std::to_string(Stockpile[19]).c_str(), 32, int(Width * 0.24), int(Height * 0.395));
	LabelArr[10] = new Label(r, std::to_string(Stockpile[21]).c_str(), 32, int(Width * 0.365), int(Height * 0.395));
	LabelArr[11] = new Label(r, std::to_string(Stockpile[28]).c_str(), 32, int(Width * 0.49), int(Height * 0.395));
	LabelArr[12] = new Label(r, std::to_string(Stockpile[20]).c_str(), 32, int(Width * 0.615), int(Height * 0.395));
	LabelArr[13] = new Label(r, std::to_string(Stockpile[27]).c_str(), 32, int(Width * 0.24), int(Height * 0.49));
	LabelArr[14] = new Label(r, std::to_string(Stockpile[25]).c_str(), 32, int(Width * 0.365), int(Height * 0.49));
	LabelArr[15] = new Label(r, std::to_string(Stockpile[7]).c_str(), 32, int(Width * 0.49), int(Height * 0.49));
	LabelArr[16] = new Label(r, std::to_string(Stockpile[24]).c_str(), 32, int(Width * 0.615), int(Height * 0.49));
	LabelArr[17] = new Label(r, std::to_string(Stockpile[22]).c_str(), 32, int(Width * 0.74), int(Height * 0.44));
	LabelArr[18] = new Label(r, std::to_string(Stockpile[9]).c_str(), 32, int(Width * 0.24), int(Height * 0.63));
	LabelArr[19] = new Label(r, std::to_string(Stockpile[8]).c_str(), 32, int(Width * 0.365), int(Height * 0.63));
	LabelArr[20] = new Label(r, std::to_string(Stockpile[26]).c_str(), 32, int(Width * 0.49), int(Height * 0.63));
	LabelArr[21] = new Label(r, std::to_string(Stockpile[12]).c_str(), 32, int(Width * 0.615), int(Height * 0.63));
	LabelArr[22] = new Label(r, std::to_string(Stockpile[23]).c_str(), 32, int(Width * 0.74), int(Height * 0.63));
	LabelArr[23] = new Label(r, std::to_string(Stockpile[16]).c_str(), 32, int(Width * 0.24), int(Height * 0.765));
	LabelArr[24] = new Label(r, std::to_string(Stockpile[13]).c_str(), 32, int(Width * 0.365), int(Height * 0.765));
	LabelArr[25] = new Label(r, std::to_string(Stockpile[17]).c_str(), 32, int(Width * 0.49), int(Height * 0.765));
	LabelArr[26] = new Label(r, std::to_string(Stockpile[15]).c_str(), 32, int(Width * 0.615), int(Height * 0.765));
	LabelArr[27] = new Label(r, std::to_string(Stockpile[18]).c_str(), 32, int(Width * 0.74), int(Height * 0.765));
	LabelArr[28] = new Label(r, std::to_string(Stockpile[14]).c_str(), 32, int(Width * 0.49), int(Height * 0.86));
	LabelArr[29] = new Label(r, std::to_string(Stockpile[29]).c_str(), 32, int(Width * 0.365), int(Height * 0.86));
	LabelArrtop = 30;
	ImageArrtop = 0;
	SliderArrtop = 0;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;
}

void IndustryScreen::UpdateText(int Stockpile[29]){
	LabelArr[0]->ChangeText(std::to_string(Stockpile[0]).c_str());
	LabelArr[1]->ChangeText(std::to_string(Stockpile[5]).c_str());
	LabelArr[2]->ChangeText(std::to_string(Stockpile[2]).c_str());
	LabelArr[3]->ChangeText(std::to_string(Stockpile[4]).c_str());
	LabelArr[4]->ChangeText(std::to_string(Stockpile[3]).c_str());
	LabelArr[5]->ChangeText(std::to_string(Stockpile[1]).c_str());
	LabelArr[6]->ChangeText(std::to_string(Stockpile[11]).c_str());
	LabelArr[7]->ChangeText(std::to_string(Stockpile[10]).c_str());
	LabelArr[8]->ChangeText(std::to_string(Stockpile[6]).c_str());
	LabelArr[9]->ChangeText(std::to_string(Stockpile[19]).c_str());
	LabelArr[10]->ChangeText(std::to_string(Stockpile[21]).c_str());
	LabelArr[11]->ChangeText(std::to_string(Stockpile[28]).c_str());
	LabelArr[12]->ChangeText(std::to_string(Stockpile[20]).c_str());
	LabelArr[13]->ChangeText(std::to_string(Stockpile[27]).c_str());
	LabelArr[14]->ChangeText(std::to_string(Stockpile[25]).c_str());
	LabelArr[15]->ChangeText(std::to_string(Stockpile[7]).c_str());
	LabelArr[16]->ChangeText(std::to_string(Stockpile[24]).c_str());
	LabelArr[17]->ChangeText(std::to_string(Stockpile[22]).c_str());
	LabelArr[18]->ChangeText(std::to_string(Stockpile[9]).c_str());
	LabelArr[19]->ChangeText(std::to_string(Stockpile[8]).c_str());
	LabelArr[20]->ChangeText(std::to_string(Stockpile[26]).c_str());
	LabelArr[21]->ChangeText(std::to_string(Stockpile[12]).c_str());
	LabelArr[22]->ChangeText(std::to_string(Stockpile[23]).c_str());
	LabelArr[23]->ChangeText(std::to_string(Stockpile[16]).c_str());
	LabelArr[24]->ChangeText(std::to_string(Stockpile[13]).c_str());
	LabelArr[25]->ChangeText(std::to_string(Stockpile[17]).c_str());
	LabelArr[26]->ChangeText(std::to_string(Stockpile[15]).c_str());
	LabelArr[27]->ChangeText(std::to_string(Stockpile[18]).c_str());
	LabelArr[28]->ChangeText(std::to_string(Stockpile[14]).c_str());
	LabelArr[29]->ChangeText(std::to_string(Stockpile[29]).c_str());
}
