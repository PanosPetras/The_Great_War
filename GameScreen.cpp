#include "PlayerController.h"
#include "ScreenList.h"
#include "SDL_ColorDetection.h"
#include "UI.h"
#include "MainWindow.h"
#include "Button.h"
#include "Label.h"
#include "Image.h"

#include <cmath>
#include <memory>

GameScreen::GameScreen(SDL_Renderer* r, const char* tag,  std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(r) {
	bHasBackground = true;
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

	PC = std::make_unique<PlayerController>(renderer, tag);
	overlay = std::make_unique<UI>(r, tag, PC.get(), [this](std::unique_ptr<Screen> scr, std::string ID) { ChangeActiveScreen(std::move(scr), std::move(ID)); });
}

void GameScreen::Pause() {
	if (StateViewingScreen) {
		CloseScreenPreview();
		//overlay->Buttons[0]->Playsound();
	} else if (bHasActiveScreen() == true){
		CloseActiveScreen();
		overlay->Buttons[0]->Playsound();
	} else {
		if (bIsPaused == true) {
			bIsPaused = false;
                        PM.reset();
		}
		else {
			PM = std::make_unique<PauseMenu>(renderer, QuitFunc, [this]{ Pause(); }, ChangeScreenFunc);
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
	if (bHasActiveScreen() == false) {
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
			static_cast<IndustryScreen*>(ActiveScreen.get())->UpdateText(Res);
		} else if (ScreenID == "EconomyScreen") {
			static_cast<EconomyScreen*>(ActiveScreen.get())->Update();
		}

		ActiveScreen->Render();
	}

	//Calls the render method for every active image
	for (auto& image : ImageArr) {
		image->Draw();
	}

	//Calls the render method for every active button
	for (auto& drawable : InputDrawableArr) {
		drawable->Draw();
	}

	//Calls the render method for every active label
	for (auto& label : LabelArr) {
		label->Draw();
	}

	//Render the pause menu if the game is paused
	if (bIsPaused == true) {
		PM->Render();
	}
	overlay->Render();

	if (StateViewingScreen) {
		StateViewingScreen->Render();
	}
}

void GameScreen::Handle_Input(SDL_Event& ev) {
	bool flag = false;
	if (StateViewingScreen) {
		StateViewingScreen->Handle_Input(ev);
		if (!StateViewingScreen) {
			flag = true;
		}
	}

	if (bHasActiveScreen() == true) {
		ActiveScreen->Handle_Input(ev);
	}

	//Handle clicks on the map
	if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
		if (bHasActiveScreen() == false && flag == false && ev.button.y > GetWindowHeight() * 0.07 && bIsPaused == false && !StateViewingScreen) {

			int x = Cam_Width + int(ev.button.x / factor) - 5384;
			int y = Cam_Height + int(ev.button.y / factor);

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
				auto close = [this]{ Pause(); };
				auto change = [this](std::unique_ptr<Screen> NewScreen, std::string ID) { ChangeActiveScreen(std::move(NewScreen), std::move(ID)); };

				//Create the StatePreview screen
                                StateViewingScreen = std::make_unique<StatePreview>(renderer, state->State_ID - 1, state->State_Name, state->State_Controller, PC.get(), res, int(state->State_Population), fcs, close, change);
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
	if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
		this->Pause();
		SDL_Delay(200);
	}

	//Handles inputs for buttons if the game is not paused
	if (not bIsPaused) {
                for(auto& drawable : InputDrawableArr) {
                        std::cerr << "GameScreen::Handle_Input\t" << static_cast<void*>(drawable.get()) << std::endl;
			drawable->HandleInput(ev);
		}

		//Handle input events as the ui
		overlay->Handle_Input(ev);

	} else {
		//Triggers the handle input event in the pause menu, if the game is paused
		PM->Handle_Input(ev);
	}
	HandleMouseMovement(ev);
}

void GameScreen::HandleMouseMovement(SDL_Event& ev) {
	if (bHasActiveScreen() == false) {
		/*Checks whether the mouse is pressed or not so that we
		can move the camera when it is pressed*/
		if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_MIDDLE && mousepressed == false) {
			mousepressed = true;
		}
		else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_MIDDLE && mousepressed == true) {
			mousepressed = false;
		}
		//Requests the mouse movement
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);

		//Moves the camera upwards
		int lim1 = int((int(ImgSize[1] * factor) - GetWindowHeight()) / factor);

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
				lim1 = int(11000 - GetWindowWidth() / 2 / factor);
				
				Cam_Width += int((MouseSensitivity * x * -1) / factor);
				if (Cam_Width > lim1) {
					Cam_Width -= 5616;
				}
			}
		}

		//Change the screen's magnification, albeit the zoom factor
		if (ev.type == SDL_MOUSEWHEEL) {
			//Zoom in
			if (ev.wheel.y > 0 && factor < GetWindowWidth() / 480.0) {
				factor += ZoomingSpeed * factor;
				Cam_Width += int(GetWindowWidth() / factor * ZoomingSpeed / 2);
				Cam_Height += int(GetWindowHeight() / factor * ZoomingSpeed / 2);
			}
			//Zoom out
			else if (factor > GetWindowWidth() / 3840.0 && ev.wheel.y < 0) {
				factor -= ZoomingSpeed * factor;

				//Make sure we are not off the limits
				if (factor < GetWindowWidth() / 3840.0) {
					factor = float(GetWindowWidth() / 3840.0);
				}

				Cam_Width -= int(GetWindowWidth() / factor * ZoomingSpeed / 2);
				Cam_Height -= int(GetWindowHeight() / factor * ZoomingSpeed / 2);

				/*If the zoomed out image extends out of the rendered image's bounds,
				then we move the camera towards the center of the rendered image*/
				int lim = int((std::trunc(ImgSize[1] * factor) - GetWindowHeight()) / factor);
				if (Cam_Height < 0) {
					Cam_Height = 0;
				} else if (Cam_Height > lim) {
					Cam_Height = lim;
				}
				lim = int((trunc(ImgSize[0] * factor) - GetWindowWidth()) / factor);
				if (Cam_Width < 0) {
					Cam_Width = 0;
				} else if (Cam_Width > lim) {
					Cam_Width = lim;
				}
			}
		}
	}
}

void GameScreen::ChangeActiveScreen(std::unique_ptr<Screen> NewScreen, std::string ID){
	if (bHasActiveScreen() == false) {
		if (StateViewingScreen) {
			CloseScreenPreview();
		}
	}
	ScreenID = std::move(ID);
	ActiveScreen = std::move(NewScreen);
}

void GameScreen::CloseActiveScreen(){
	if (bHasActiveScreen() == true) {
                ActiveScreen.reset();
		ScreenID = "0";
	}
}

void GameScreen::CloseScreenPreview(){
        StateViewingScreen.reset();

	SDL_Surface* base = SDL_CreateRGBSurface(0, 16383, 2160, 32, 0xff, 0xff00, 0xff0000, 0xff000000);

	SDL_Texture* txt = SDL_CreateTextureFromSurface(renderer, base);
	SDL_DestroyTexture(PC->overlay);
	PC->overlay = txt;
	SDL_FreeSurface(base);
}
