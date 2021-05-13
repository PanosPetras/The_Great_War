#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Country.h"
#include "State.h"
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <future>
#include <fstream>
#include <vector>
#include "SDL_ColorDetection.h"

class PlayerController {
public:

	//This is the representing the pass of a single day
	void Tick();

	//Constructor
	PlayerController(SDL_Renderer* r, const char* tag);
	
	//Destructor
	~PlayerController();

	//Advances the date by one day
	static int AdvanceDate(void* ref);

	//Pauses the flow of time
	void Pause();

	//Changes the game speed
	void ChangeSpeed(bool change);

	//Reference to every single state on the map
	State* StatesArr[2703];

	//Reference to every country
	Country* CountriesArr[59];

	SDL_Texture* txt;
	SDL_Texture* overlay;
	SDL_Surface* ormap;
	SDL_Surface* map;
	SDL_Renderer* RendererReference;

	SDL_Surface* provinces;

	SDL_Thread* thread;

	std::string player_tag;
	int player_index;

	struct {
		int Year;
		int Month;
		int Day;
		int Speed;
		bool bIsPaused;
		int MonthDays[12];
	} Date;
};
#endif