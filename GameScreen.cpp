#include "ScreenList.h"
#include "SDL_ColorDetection.h"

GameScreen::GameScreen(SDL_Renderer* r, int Width, int Height, const char* tag,  std::function<void()> fp, std::function<void(Screen*)> fpl) : Screen(r, Width, Height) {
	bHasBackground = true;
	ButtonArrtop = 0;
	LabelArrtop = 0;
	ImageArrtop = 0;
	SliderArrtop = 0;
	bIsPaused = false;
	bZoom = true;
	factor = 1.0f;
	Cam_Height = 150;
	Cam_Width = 5384 + 2150;
	ImgSize[0] = 16383;
	ImgSize[1] = 2160;
	ZoomingSpeed = 0.1f;
	mousepressed = false;
	MouseSensitivity = 3;
	ChangeScreenFunc = fpl;
	QuitFunc = fp;

	PC = new PlayerController(renderer, tag);
	auto change = std::bind(&GameScreen::ChangeActiveScreen, this, std::placeholders::_1, std::placeholders::_2);
	overlay = new UI(r, Width, Height, tag, PC, change);

	StateViewingScreen = nullptr;
	bHasStatePreview = false;

	bHasActiveScreen = false;
}

GameScreen::~GameScreen(){
	delete PC, overlay;

	//Delete all items created by the screen in order to avoid memory leaks
	for (int x = 0; x < ButtonArrtop; x++) {
		delete ButtonArr[x];
	}

	for (int x = 0; x < LabelArrtop; x++) {
		delete LabelArr[x];
	}

	for (int x = 0; x < ImageArrtop; x++) {
		delete ImageArr[x];
	}

	if (bIsPaused == true) {
		delete PM;
	}
	if (bHasActiveScreen == true) {
		delete ActiveScreen;
	}
}

void GameScreen::Pause() {
	if (bHasStatePreview == true) {
		CloseScreenPreview();
		//overlay->Buttons[0]->Playsound();
	} else if (bHasActiveScreen == true){
		CloseActiveScreen();
		overlay->Buttons[0]->Playsound();
	} else {
		if (bIsPaused == true) {
			bIsPaused = false;
			delete PM;
		}
		else {
			auto unpause = std::bind(&GameScreen::Pause, this);
			PM = new PauseMenu(renderer, WindowSize[0], WindowSize[1], QuitFunc, unpause, ChangeScreenFunc);
			bIsPaused = true;
			if (PC->Date.bIsPaused == false) {
				overlay->PauseDate(true);
			}
			overlay->Buttons[0]->Playsound();
		}
	}
}

void GameScreen::RenderBackground() {
	/*Check if the rendered Image must be zoomed.
	If it musn't, then we just cope the image to the surface.
	If it does, then we create a rectangle and give it the
	appropriate dimensions, based on the magnification
	factor reiceived from user input*/
	if (bZoom == true) {
		SDL_Rect dstrect = { int(Cam_Width * -1 * factor), int(Cam_Height * -1 * factor), int(ImgSize[0] * factor) , int(factor * ImgSize[1]) };
		SDL_RenderCopy(renderer, PC->txt, NULL, &dstrect);
		SDL_RenderCopy(renderer, PC->overlay, NULL, &dstrect);
	}
}

void GameScreen::Render() {
	//Calls the method responsible for rendering the background
	if (bHasActiveScreen == false) {
		this->RenderBackground();
	} else {
		if (/*overlay->bDateUpdated == true && */ScreenID == "IndustryScreen") {
			int Res[30] = { PC->CountriesArr.at(PC->player_index)->Stock.Coal,
				PC->CountriesArr.at(PC->player_index)->Stock.Oil,
				PC->CountriesArr.at(PC->player_index)->Stock.Timber,
				PC->CountriesArr.at(PC->player_index)->Stock.Rubber,
				PC->CountriesArr.at(PC->player_index)->Stock.Cotton,
				PC->CountriesArr.at(PC->player_index)->Stock.Iron,
				PC->CountriesArr.at(PC->player_index)->Stock.Grain,
				PC->CountriesArr.at(PC->player_index)->Stock.Fruit,
				PC->CountriesArr.at(PC->player_index)->Stock.Electric_gear,
				PC->CountriesArr.at(PC->player_index)->Stock.Machine_parts,
				PC->CountriesArr.at(PC->player_index)->Stock.Glass,
				PC->CountriesArr.at(PC->player_index)->Stock.Lumber,
				PC->CountriesArr.at(PC->player_index)->Stock.Cement,
				PC->CountriesArr.at(PC->player_index)->Stock.Ammunition,
				PC->CountriesArr.at(PC->player_index)->Stock.Planes,
				PC->CountriesArr.at(PC->player_index)->Stock.Explosives,
				PC->CountriesArr.at(PC->player_index)->Stock.Small_arms,
				PC->CountriesArr.at(PC->player_index)->Stock.Artillery,
				PC->CountriesArr.at(PC->player_index)->Stock.Tanks,
				PC->CountriesArr.at(PC->player_index)->Stock.Canned_food,
				PC->CountriesArr.at(PC->player_index)->Stock.Furniture,
				PC->CountriesArr.at(PC->player_index)->Stock.Clothes,
				PC->CountriesArr.at(PC->player_index)->Stock.Automobiles,
				PC->CountriesArr.at(PC->player_index)->Stock.Merchant_ships,
				PC->CountriesArr.at(PC->player_index)->Stock.Radios,
				PC->CountriesArr.at(PC->player_index)->Stock.Telephones,
				PC->CountriesArr.at(PC->player_index)->Stock.Fuel,
				PC->CountriesArr.at(PC->player_index)->Stock.Paper,
				PC->CountriesArr.at(PC->player_index)->Stock.Liquor,
				PC->CountriesArr.at(PC->player_index)->Stock.Airship };
			static_cast<IndustryScreen*>(ActiveScreen)->UpdateText(Res);
		} else if (ScreenID == "EconomyScreen") {
			static_cast<EconomyScreen*>(ActiveScreen)->Update();
		}

		ActiveScreen->Render();
	}

	//Calls the render method for every active image
	for (int x = 0; x < ImageArrtop; x++) {
		ImageArr[x]->Draw();
	}

	//Calls the render method for every active button
	for (int x = 0; x < ButtonArrtop; x++) {
		ButtonArr[x]->Draw();
	}

	//Calls the render method for every active label
	for (int x = 0; x < LabelArrtop; x++) {
		LabelArr[x]->Draw();
	}

	//Render the pause menu if the game is paused
	if (bIsPaused == true) {
		PM->Render();
	}
	overlay->Render();

	if (bHasStatePreview == true) {
		StateViewingScreen->Render();
	}
}

void GameScreen::Handle_Input(SDL_Event* ev) {
	bool flag = false;
	if (bHasStatePreview) {
		StateViewingScreen->Handle_Input(ev);
		if (!bHasStatePreview) {
			flag = true;
		}
	}

	if (bHasActiveScreen == true) {
		ActiveScreen->Handle_Input(ev);
	}

	//Handle clicks on the map
	if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) {
		if (bHasActiveScreen == false &&
			flag == false &&
			ev->button.y > WindowSize[1] * 0.07 &&
			bIsPaused == false && bHasStatePreview == false) {

			int x = Cam_Width + int(ev->button.x / factor) - 5384;
			int y = Cam_Height + int(ev->button.y / factor);

			Color rgb = CD::getcolor(PC->provinces, x, y);

			if (PC->StatesMap.contains(rgb.toString())) {
				State* state = PC->StatesMap.find(rgb.toString())->second;

				std::string fcs[4];
				for (int i = 0; i < 4; i++) {
					if (state->State_Factories[i] != nullptr) {
						fcs[i] = state->State_Factories[i]->Type;
					}
					else {
						fcs[i] = "";
					}
				}

				//Access the state's resources
				int res[8] = { state->Resources.Coal, state->Resources.Cotton, state->Resources.Fruit, state->Resources.Grain, state->Resources.Iron, state->Resources.Oil, state->Resources.Rubber, state->Resources.Timber };
				auto close = std::bind(&GameScreen::Pause, this);

				//Create the StatePreview screen
				if (bHasStatePreview == false) {
					StateViewingScreen = new StatePreview(renderer, WindowSize[0], WindowSize[1], state->State_ID - 1, state->State_Name, state->State_Controller, PC, res, int(state->State_Population), fcs, close);
					bHasStatePreview = true;
				}
				else {
					delete StateViewingScreen;
					StateViewingScreen = new StatePreview(renderer, WindowSize[0], WindowSize[1], state->State_ID - 1, state->State_Name, state->State_Controller, PC, res, int(state->State_Population), fcs, close);
				}
			}

			SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);

			SDL_Surface* Marker = IMG_Load("Icons/pin1.png");

			SDL_Rect strect = { .x = -x - 5384 + 7, .y = -y + 24, .w = 5616 * 3 , .h = 2160 };
			SDL_BlitSurface(Marker, &strect, base, NULL);

			SDL_Texture* txt = SDL_CreateTextureFromSurface(renderer, base);
			SDL_DestroyTexture(PC->overlay);
			PC->overlay = txt;
			SDL_FreeSurface(base);
			SDL_FreeSurface(Marker);
		}
	}

	//Pause game if esc is pressed
	if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_ESCAPE) {
		this->Pause();
		SDL_Delay(200);
	}

	//Handles inputs for buttons if the game is not paused
	if (bIsPaused == false) {
		for (int x = 0; x < ButtonArrtop; x++) {
			ButtonArr[x]->HandleInput(ev);
		}

		//Handle input events as the ui
		overlay->Handle_Input(ev);

	} else {
		//Triggers the handle input event in the pause menu, if the game is paused
		PM->Handle_Input(ev);
	}
	HandleMouseMovement(ev);
}

void GameScreen::HandleMouseMovement(SDL_Event* ev) {
	if (bHasActiveScreen == false) {
		/*Checks whether the mouse is pressed or not so that we
		can move the camera when it is pressed*/
		if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_MIDDLE && mousepressed == false) {
			mousepressed = true;
		}
		else if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_MIDDLE && mousepressed == true) {
			mousepressed = false;
		}
		//Requests the mouse movement
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);

		//Moves the camera upwards
		int lim1 = int((int(ImgSize[1] * factor) - WindowSize[1]) / factor);

		if (mousepressed) {
			if (y > 0 && Cam_Height > 0) {
				Cam_Height -= int((MouseSensitivity * y) / factor);
				if (Cam_Height < 0) {
					Cam_Height = 0;
				}
			}
			//Moves the camera downwards
			else if (y < 0 && Cam_Height < lim1) {
				Cam_Height += int((MouseSensitivity * y * -1) / factor);
				if (Cam_Height > lim1) {
					Cam_Height = lim1;
				}
			}

			//Moves the camera to the left
			if (x > 0 && Cam_Width > 0) {
				Cam_Width -= int((MouseSensitivity * x) / factor);
				if (Cam_Width < 5384) {
					Cam_Width += 5616;
				}
			}
			//Moves the camera to the right
			else if (x < 0 && Cam_Width) {
				lim1 = int(11000 - WindowSize[0] / 2 / factor);
				
				Cam_Width += int((MouseSensitivity * x * -1) / factor);
				if (Cam_Width > lim1) {
					Cam_Width -= 5616;
				}
			}
		}

		//Change the screen's magnification, albeit the zoom factor
		if (ev->type == SDL_MOUSEWHEEL) {
			//Zoom in
			if (ev->wheel.y > 0 && factor < WindowSize[0] / 480.0) {
				factor += ZoomingSpeed * factor;
				Cam_Width += int(WindowSize[0] / factor * ZoomingSpeed / 2);
				Cam_Height += int(WindowSize[1] / factor * ZoomingSpeed / 2);
			}
			//Zoom out
			else if (factor > WindowSize[0] / 3840.0 && ev->wheel.y < 0) {
				factor -= ZoomingSpeed * factor;

				//Make sure we are not off the limits
				if (factor < WindowSize[0] / 3840.0) {
					factor = float(WindowSize[0] / 3840.0);
				}

				Cam_Width -= int(WindowSize[0] / factor * ZoomingSpeed / 2);
				Cam_Height -= int(WindowSize[1] / factor * ZoomingSpeed / 2);

				/*If the zoomed out image extends out of the rendered image's bounds,
				then we move the camera towards the center of the rendered image*/
				int lim = int((trunc(ImgSize[1] * factor) - WindowSize[1]) / factor);
				if (Cam_Height < 0) {
					Cam_Height = 0;
				} else if (Cam_Height > lim) {
					Cam_Height = lim;
				}
				lim = int((trunc(ImgSize[0] * factor) - WindowSize[0]) / factor);
				if (Cam_Width < 0) {
					Cam_Width = 0;
				} else if (Cam_Width > lim) {
					Cam_Width = lim;
				}
			}
		}
	}
}

void GameScreen::ChangeActiveScreen(Screen* NewScreen, std::string ID){
	if (bHasActiveScreen == false) {
		bHasActiveScreen = true;
		if (bHasStatePreview == true) {
			CloseScreenPreview();
		}
	} else {
		delete ActiveScreen;
	}
	ScreenID = ID;
	ActiveScreen = NewScreen;
}

void GameScreen::CloseActiveScreen(){
	if (bHasActiveScreen == true) {
		bHasActiveScreen = false;
		delete ActiveScreen;
		ScreenID = "0";
	}
}

void GameScreen::CloseScreenPreview(){
	delete StateViewingScreen;
	bHasStatePreview = false;

	SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);

	SDL_Texture* txt = SDL_CreateTextureFromSurface(renderer, base);
	SDL_DestroyTexture(PC->overlay);
	PC->overlay = txt;
	SDL_FreeSurface(base);
}
