#include "PlayerController.h"
#include "ScreenList.h"
#include "WindowInfo.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, PlayerController* PC) : DiplomacyScreen(r, PC, PC->player_tag) {
}

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, PlayerController* PC, std::string targetTag) : Screen(r) {
	bHasBackground = true;
	SetupBg("Backgrounds/Industry.png");

	int fontSize = 26, Width = GetWindowWidth(), Height = GetWindowHeight();

	ImageArr[0] = new Image(r, "Backgrounds/old_paper.png", int(Width * 0.4), int(Height * 0.2), int(Width * 0.5), int(Height * 0.6));

	ImageArr[1] = new Image(r, std::string("Flags/") + targetTag + ".png", int(Width * 0.4), int(Height * 0.2), 120, 80);
	ImageArr[2] = new Image(r, "Icons/population.png", int(Width * 0.43), int(Height * 0.3), int(Width * 0.027), int(Height * 0.048));
	ImageArr[3] = new Image(r, "Icons/flags.png", int(Width * 0.77), int(Height * 0.225), int(Width * 0.027), int(Height * 0.048));

	InputDrawableArrtop = CreateCountryButtons(PC, targetTag);

	LabelArr[0] = new Label(r, PC->CountriesArr[selectedCountryIndex]->GetName(), 32, int(Width * 0.48), int(Height * 0.218), Width);
	LabelArr[1] = new Label(r, std::to_string(PC->CountriesArr[selectedCountryIndex]->GetPopulation()), 24, int(Width * 0.465), int(Height * 0.31));
	LabelArr[2] = new Label(r, "N/A", 28, int(Width * 0.8), int(Height * 0.225));

	InputDrawableArr[InputDrawableArrtop++] = new Button(r, int(Width * 0.43), int(Height * 0.5), int(Width * 0.13), int(Height * 0.04), "Declare War", fontSize);
	auto action = std::bind(&DiplomacyScreen::ImproveRelations, this);
	InputDrawableArr[InputDrawableArrtop++] = new Button(r, int(Width * 0.43), int(Height * 0.58), int(Width * 0.13), int(Height * 0.04), "Improve Relations", fontSize, action);
	action = std::bind(&DiplomacyScreen::WorsenRelations, this);
	InputDrawableArr[InputDrawableArrtop++] = new Button(r, int(Width * 0.43), int(Height * 0.66), int(Width * 0.13), int(Height * 0.04), "Worsen Relations", fontSize, action);
	action = std::bind(&DiplomacyScreen::SendAllianceRequest, this);
	InputDrawableArr[InputDrawableArrtop++] = new Button(r, int(Width * 0.645), int(Height * 0.5), int(Width * 0.13), int(Height * 0.04), "Form Alliance", fontSize, action);
	action = std::bind(&DiplomacyScreen::ImposeEmbargo, this);
	InputDrawableArr[InputDrawableArrtop++] = new Button(r, int(Width * 0.645), int(Height * 0.58), int(Width * 0.13), int(Height * 0.04), "Embargo", fontSize, action);

	LabelArrtop = 3;
	ImageArrtop = 4;

	PCref = PC;

	UpdateRelationValue();
	UpdateAllianceState();
}

DiplomacyScreen::DiplomacyScreen(SDL_Renderer* r, PlayerController* PC, int index) : DiplomacyScreen(r, PC){
	SelectCountry((void*)((Uint64)index));
}

int DiplomacyScreen::CreateCountryButtons(PlayerController* PC) {
	int i = 0, flagsPerLine = 12, Width = GetWindowWidth(), Height = GetWindowHeight();

	for (auto country : PC->CountriesArr) {
		std::function<void(void*)> func = std::bind(&DiplomacyScreen::SelectCountry, this, std::placeholders::_1);
		InputDrawableArr[i] = new Button(renderer, int(Width * (0.1 + (i / flagsPerLine) * 0.05)), int(Height * (0.17 + (i % flagsPerLine) * 0.06)), 60, 40, ("Flags/" + country->GetTag()).c_str(), func, (void*)((Uint64)i));
		i++;
	}

	return i;
}

int DiplomacyScreen::CreateCountryButtons(PlayerController* PC, std::string targetTag) {
	int i = 0, flagsPerLine = 12, Width = GetWindowWidth(), Height = GetWindowHeight(), index = 0;

	for (auto country : PC->CountriesArr) {
		std::function<void(void*)> func = std::bind(&DiplomacyScreen::SelectCountry, this, std::placeholders::_1);
		InputDrawableArr[i] = new Button(renderer, int(Width * (0.1 + (i / flagsPerLine) * 0.05)), int(Height * (0.17 + (i % flagsPerLine) * 0.06)), 60, 40, ("Flags/" + country->GetTag()).c_str(), func, (void*)((Uint64)i));
		if (country->GetTag() == targetTag) {
			selectedCountryIndex = i;
		}
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
	UpdateEmbargoState();
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

void DiplomacyScreen::ImposeEmbargo() {
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];

	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		if (!rel->second.GetIfHasEmbargo(PCref->player_tag)) {
			rel->second.ImposeEmbargo(PCref->player_tag);
		}
		else {
			rel->second.LiftEmbargo(PCref->player_tag);
		}
	}

	UpdateEmbargoState();
	UpdateRelationValue();
}

void DiplomacyScreen::SendAllianceRequest() {
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];

	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		if (!rel->second.GetIfAllied()) {
			PCref->CountriesArr[selectedCountryIndex]->AddRequest(Request(alliance, PCref->player_index, PCref->CountriesArr[PCref->player_index]->GetTag(), rel->second));
		} else {
			rel->second.BreakAlliance();

			UpdateAllianceState();
			UpdateRelationValue();
		}
	}
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
			((Button*)InputDrawableArr[InputDrawableArrtop - 2])->ChangeText("Break Alliance", 26);
			return;
		}
	}
	((Button*)InputDrawableArr[InputDrawableArrtop - 2])->ChangeText("Form Alliance", 26);
}

void DiplomacyScreen::UpdateEmbargoState() {
	Country* c1 = PCref->CountriesArr[PCref->player_index], * c2 = PCref->CountriesArr[selectedCountryIndex];

	auto rel = PCref->diplo.relations->find(CountryPair(c1, c2));

	if (rel != PCref->diplo.relations->end()) {
		if (rel->second.GetIfHasEmbargo(PCref->player_tag)) {
			((Button*)InputDrawableArr[InputDrawableArrtop - 1])->ChangeText("Lift Embargo", 26);
			return;
		}
	}
	((Button*)InputDrawableArr[InputDrawableArrtop - 1])->ChangeText("Embargo", 26);
}