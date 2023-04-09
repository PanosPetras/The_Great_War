#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Country.h"
#include "Diplomacy.h"
#include "State.h"

#include <SDL.h>

#include <memory>
#include <unordered_map>

typedef std::unique_ptr<std::vector<std::string>, std::default_delete<std::vector<std::string>>> VectorSmartPointer;

class PlayerController {
public:
	//Constructor
	PlayerController(SDL_Renderer* r, const char* tag);

        ~PlayerController();

private:
	//Loading data functions
	VectorSmartPointer LoadCountryNames(std::ifstream&);
	VectorSmartPointer LoadCountryTags(std::ifstream&);
	int* LoadCountriesBalance(std::ifstream&);
	VectorSmartPointer LoadStateNames(std::ifstream&);
	VectorSmartPointer LoadStateOwnerTags(std::ifstream&);
	unsigned char(*LoadStateColors(std::ifstream&))[3];
	short (*LoadStateCoordinates(std::ifstream&))[2];
	int* LoadStatePops(std::ifstream&);
	void InitializeCountries(VectorSmartPointer& names, VectorSmartPointer& tags, const char* tag, int* balance);
	void InitializeStates(VectorSmartPointer& owners, VectorSmartPointer& names, short(*coords)[2], int* populations, unsigned char(*colors)[3]);

	static int LoadMap(void*);
	static int LoadUtilityAssets(void*);
	
public:

	//Some info about the player
	std::string player_tag;
	int player_index;

	//The in-game date
	struct {
		int Year;
		int Month;
		int Day;
		int Speed;
		bool bIsPaused;
		int MonthDays[12];
	} Date;

	//This is the representing the pass of a single day
	void Tick();

	//Advances the date by one day
	static int AdvanceDate(void* ref);

	//Pauses the flow of time
	void Pause();

	//Changes the game speed
	void ChangeSpeed(bool change);

	Market WorldMarket;

	//Reference to every single state on the map
	State** StatesArr;
	std::unordered_map<std::string, State*> StatesMap;

	//Reference to every country
	std::vector<Country*> CountriesArr;

	//The state of the diplomatic relations between every country
	Diplomacy diplo;

	//Some SDL assets needed
	SDL_Renderer* RendererReference;
	SDL_Texture* txt;

	SDL_Texture* overlay;
	SDL_Surface* map;
	SDL_Surface* provinces;

	//Used to run the time-functionality of the game
	SDL_Thread* thread;
};
#endif
