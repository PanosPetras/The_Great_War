#include "ScreenList.h"
#include "WindowInfo.h"

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, PlayerController* PC) : Screen(r){
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");

	int fontSize = 26, Width = GetWindowWidth(), Height = GetWindowHeight();

	ImageArr[0] = new Image(r, "Backgrounds/old_paper.png", int(Width * 0.4), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));

	ImageArr[1] = new Image(r, std::string("Flags/") + PC->player_tag + ".png", int(Width * 0.4), int(Height * 0.2), 120, 80);
	ImageArr[2] = new Image(r, "Icons/population.png", int(Width * 0.43), int(Height * 0.3), int(Width * 0.027), int(Height * 0.048));
	ImageArr[3] = new Image(r, "Icons/flags.png", int(Width * 0.77), int(Height * 0.225), int(Width * 0.027), int(Height * 0.048));

	LabelArr[0] = new Label(r, PC->CountriesArr[PC->player_index]->GetName(), 32, int(Width * 0.48), int(Height * 0.218));
	LabelArr[1] = new Label(r, std::to_string(PC->CountriesArr[PC->player_index]->GetPopulation()), 24, int(Width * 0.465), int(Height * 0.31));
	LabelArr[2] = new Label(r, "N/A", 28, int(Width * 0.8), int(Height * 0.225));

	ButtonArrtop = CreateCountryButtons(PC);

	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.445), int(Height * 0.5), int(Width * 0.11), int(Height * 0.04), "Declare War", fontSize);
	auto change = std::bind(&DiplomacyScreen::ImproveRelations, this);
	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.43), int(Height * 0.58), int(Width * 0.14), int(Height * 0.04), "Improve Relations", fontSize, change);
	change = std::bind(&DiplomacyScreen::WorsenRelations, this);
	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.431), int(Height * 0.66), int(Width * 0.138), int(Height * 0.04), "Worsen Relations", fontSize, change);

	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.635), int(Height * 0.5), int(Width * 0.13), int(Height * 0.04), "Form Alliance", fontSize);
	ButtonArr[ButtonArrtop++] = new Button(r, int(Width * 0.645), int(Height * 0.58), int(Width * 0.11), int(Height * 0.04), "Embargo", fontSize);

	LabelArrtop = 3;
	ImageArrtop = 4;
	SliderArrtop = 0;

	PCref = PC;

	selectedCountryIndex = PC->player_index;
}

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, int index, PlayerController* PC) : DiplomacyScreen(r, PC){
	SelectCountry((void*)((Uint64)index));
}

int DiplomacyScreen::CreateCountryButtons(PlayerController* PC) {
	int i = 0, flagsPerLine = 12, Width = GetWindowWidth(), Height = GetWindowHeight();

	for (auto country : PC->CountriesArr) {
		std::function<void(void*)> func = std::bind(&DiplomacyScreen::SelectCountry, this, std::placeholders::_1);
		ButtonArr[i] = new Button(renderer, int(Width * (0.1 + (i / flagsPerLine) * 0.05)), int(Height * (0.17 + (i % flagsPerLine) * 0.06)), 60, 40, ("Flags/" + country->GetTag()).c_str(), func, (void*)((Uint64)i));
		i++;
	}

	return i;
}

void DiplomacyScreen::SelectCountry(void* country){
	int index = (int)(Uint64)(country);

	ImageArr[1]->ChangeImage(std::string("Flags/") + PCref->CountriesArr[index]->GetTag() + ".png");
	LabelArr[0]->ChangeText(PCref->CountriesArr[index]->GetName());
	LabelArr[1]->ChangeText(std::to_string(PCref->CountriesArr[index]->GetPopulation()));

	selectedCountryIndex = index;

	UpdateRelationValue();
	UpdateAllianceState();
}

void DiplomacyScreen::ImproveRelations(){
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];
	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		rel->second.ImproveRelations();
	}

	UpdateRelationValue();
}

void DiplomacyScreen::WorsenRelations(){
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];
	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		rel->second.WorsenRelations();
	}

	UpdateRelationValue();
}

void DiplomacyScreen::UpdateRelationValue(){
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];

	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		LabelArr[2]->ChangeText(std::to_string(rel->second.GetRelationsValue()));
	} else {
		LabelArr[2]->ChangeText("N/A");
	}
}

void DiplomacyScreen::UpdateAllianceState() {
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];

	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		if (rel->second.GetIfAllied()) {
			((Button*)ButtonArr[ButtonArrtop - 2])->ChangeText("Break Alliance", 26);
		} else {
			((Button*)ButtonArr[ButtonArrtop - 2])->ChangeText("Form Alliance", 26);
		}
	}
}
