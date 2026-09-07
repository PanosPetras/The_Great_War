#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"
#include "SDL_ColorDetection.h"
#include "ScreenList.h"
#include "UI.h"

#include <cmath>
#include <iostream>
#include <memory>

GameScreen::GameScreen(MainWindow& mw, const char* tag, std::function<void()> fp, std::function<void(std::unique_ptr<Screen>)> fpl) : Screen(mw) {
    bHasBackground = true;
    ChangeScreenFunc = fpl;
    QuitFunc = fp;

    PC = std::make_unique<PlayerController>(*main_window, tag);

    // Decoded once for the life of the window, rather than on every click
    pinTexture = main_window->IMG_Load("Icons/pin1.png");

    overlay = std::make_unique<UI>(*main_window, tag, PC.get(), [this](std::unique_ptr<Screen> scr, std::string ID) { ChangeActiveScreen(std::move(scr), std::move(ID)); });
}

void GameScreen::Pause() {
    if(StateViewingScreen) {
        CloseScreenPreview();
        // overlay->Buttons[0]->Playsound();
    } else if(bHasActiveScreen() == true) {
        CloseActiveScreen();
        overlay->Buttons[0]->Playsound();
    } else {
        if(bIsPaused == true) {
            bIsPaused = false;
            PM.reset();
        } else {
            PM = std::make_unique<PauseMenu>(*main_window, QuitFunc, [this] { Pause(); }, ChangeScreenFunc);
            bIsPaused = true;
            if(PC->bIsPaused == false) {
                overlay->PauseDate(true);
            }
            overlay->Buttons[0]->Playsound();
        }
    }
}

void GameScreen::Update(Uint32 elapsedMs) {
    // The pause menu freezes the whole game, not just the calendar
    if(bIsPaused) {
        return;
    }

    PC->Update(elapsedMs);

    /*Let whichever screens are open refresh themselves against the state the
    tick above produced. Each one knows what it displays, so there is nothing
    to dispatch on here.*/
    if(ActiveScreen) {
        ActiveScreen->Update(elapsedMs);
    }

    if(StateViewingScreen) {
        StateViewingScreen->Update(elapsedMs);
    }
}

int GameScreen::WrapX(int x) {
    x %= MapWidth;
    return x < 0 ? x + MapWidth : x;
}

int GameScreen::ScreenX(int worldX) const {
    return int(std::lround((worldX - Cam_Width) * factor));
}

int GameScreen::ScreenY(int worldY) const {
    return int(std::lround((worldY - Cam_Height) * factor));
}

bool GameScreen::OnMap(int screenY) const {
    const int y = Cam_Height + int(screenY / factor);
    return y >= 0 && y < MapHeight;
}

int GameScreen::VisibleCopies() const {
    /*The camera starts somewhere inside copy 0, so covering the screen takes
    one more copy than the view is wide.*/
    return int(std::ceil(main_window->Width() / factor / MapWidth)) + 1;
}

void GameScreen::RenderMap() {
    const int top = ScreenY(0), bottom = ScreenY(MapHeight);

    for(int copy = 0; copy < VisibleCopies(); ++copy) {
        /*A copy's right edge is the next copy's left edge, computed the same
        way, so neighbours meet on the exact same pixel and leave no seam.*/
        const int left = ScreenX(copy * MapWidth), right = ScreenX((copy + 1) * MapWidth);

        SDL_Rect dstrect = {left, top, right - left, bottom - top};
        SDL_RenderCopy(*main_window, PC->txt, nullptr, &dstrect);
    }
}

void GameScreen::RenderBackground() {
    /*Check if the rendered Image must be zoomed.
    If it musn't, then we just cope the image to the surface.
    If it does, then we create a rectangle and give it the
    appropriate dimensions, based on the magnification
    factor reiceived from user input*/
    if(bZoom == true) {
        RenderMap();
        RenderPin();
    }
}

void GameScreen::RenderPin() {
    if(not pin) {
        return;
    }

    /*The pin belongs to the map rather than to the screen, so it is measured in
    map pixels and scales with the zoom, and its point lands on the pixel that
    was clicked.*/
    const int size = int(PinSize * factor);
    const int y = ScreenY(pin->y - PinPointY);

    /*The world wraps, so the pin is drawn against every copy of it the camera
    can see. The renderer clips away the ones that fall off the screen.*/
    for(int copy = 0; copy < VisibleCopies(); ++copy) {
        SDL_Rect dstrect = {ScreenX(pin->x + copy * MapWidth - PinPointX), y, size, size};
        SDL_RenderCopy(*main_window, pinTexture, nullptr, &dstrect);
    }
}

void GameScreen::Render() {
    // Calls the method responsible for rendering the background
    if(bHasActiveScreen() == false) {
        this->RenderBackground();
    } else {
        ActiveScreen->Render();
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

    // Render the pause menu if the game is paused
    if(bIsPaused == true) {
        PM->Render();
    }
    overlay->Render();

    if(StateViewingScreen) {
        StateViewingScreen->Render();
    }
}

void GameScreen::Handle_Input(SDL_Event& ev) {
    bool flag = false;
    if(StateViewingScreen) {
        StateViewingScreen->Handle_Input(ev);
        if(!StateViewingScreen) {
            flag = true;
        }
    }

    if(bHasActiveScreen() == true) {
        ActiveScreen->Handle_Input(ev);
    }

    // Handle clicks on the map
    if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
        if(bHasActiveScreen() == false && flag == false && ev.button.y > main_window->Height() * 0.07 && bIsPaused == false && !StateViewingScreen && OnMap(ev.button.y)) {
            /*The province surface is read directly, so the click has to land on
            it. Wrapping x does that for free; y has nowhere to wrap to, which
            is what OnMap above checks.*/
            const int x = WrapX(Cam_Width + int(ev.button.x / factor));
            const int y = Cam_Height + int(ev.button.y / factor);

            Color rgb(CD::getcolor(PC->provinces, x, y));

            if(PC->StatesMap.contains(rgb.toString())) {
                State* state = PC->StatesMap.find(rgb.toString())->second;

                std::string fcs[4];
                for(unsigned i = 0; i < 4; i++) {
                    if(state->State_Factories[i] != nullptr) {
                        fcs[i] = state->State_Factories[i]->Kind.name;
                    } else {
                        fcs[i] = "";
                    }
                }

                auto close = [this] { Pause(); };
                auto change = [this](std::unique_ptr<Screen> NewScreen, std::string ID) { ChangeActiveScreen(std::move(NewScreen), std::move(ID)); };

                // Create the StatePreview screen
                StateViewingScreen =
                    std::make_unique<StatePreview>(*main_window, state->State_ID - 1, state->State_Name, state->State_Controller, PC.get(), state->Resources, int(state->State_Population), fcs, close, change);
            }

            // Drop the pin on the clicked pixel, in world coordinates
            pin = SDL_Point{x, y};
        }
    }

    // Pause game if esc is pressed
    if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
        this->Pause();
        SDL_Delay(200);
    }

    // Handles inputs for buttons if the game is not paused
    if(not bIsPaused) {
        for(auto& drawable : InputDrawableArr) {
            std::cerr << "GameScreen::Handle_Input\t" << static_cast<void*>(drawable.get()) << std::endl;
            drawable->HandleInput(ev);
        }

        // Handle input events as the ui
        overlay->Handle_Input(ev);

    } else {
        // Triggers the handle input event in the pause menu, if the game is paused
        PM->Handle_Input(ev);
    }
    HandleMouseMovement(ev);
}

void GameScreen::HandleMouseMovement(SDL_Event& ev) {
    if(bHasActiveScreen() == false) {
        /*Checks whether the mouse is pressed or not so that we
        can move the camera when it is pressed*/
        if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_MIDDLE && mousepressed == false) {
            mousepressed = true;
        } else if(ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_MIDDLE && mousepressed == true) {
            mousepressed = false;
        }
        // Requests the mouse movement
        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        // Moves the camera upwards
        int lim1 = int((int(MapHeight * factor) - main_window->Height()) / factor);

        if(mousepressed) {
            if(y > 0 && Cam_Height > 0) {
                Cam_Height -= int((MouseSensitivity * y) / factor);
                if(Cam_Height < 0) {
                    Cam_Height = 0;
                }
            }
            // Moves the camera downwards
            else if(y < 0 && Cam_Height < lim1) {
                Cam_Height += int((MouseSensitivity * y * -1) / factor);
                if(Cam_Height > lim1) {
                    Cam_Height = lim1;
                }
            }

            /*Moves the camera sideways. There is no limit to run into: the
            world is a cylinder, so the camera just wraps around it.*/
            Cam_Width = WrapX(Cam_Width - int((MouseSensitivity * x) / factor));
        }

        // Change the screen's magnification, albeit the zoom factor
        if(ev.type == SDL_MOUSEWHEEL) {
            // Zoom in
            if(ev.wheel.y > 0 && factor < main_window->Width() / 480.0) {
                factor += ZoomingSpeed * factor;
                Cam_Width = WrapX(Cam_Width + int(main_window->Width() / factor * ZoomingSpeed / 2));
                Cam_Height += int(main_window->Height() / factor * ZoomingSpeed / 2);
            }
            // Zoom out
            else if(factor > main_window->Width() / 3840.0 && ev.wheel.y < 0) {
                factor -= ZoomingSpeed * factor;

                // Make sure we are not off the limits
                if(factor < main_window->Width() / 3840.0) {
                    factor = main_window->Width() / 3840.0;
                }

                Cam_Width = WrapX(Cam_Width - int(main_window->Width() / factor * ZoomingSpeed / 2));
                Cam_Height -= int(main_window->Height() / factor * ZoomingSpeed / 2);

                /*Zooming out can pull the view past the top or bottom edge of
                the map, which - unlike the sides - does not wrap.*/
                const int lim = int((std::trunc(MapHeight * factor) - main_window->Height()) / factor);
                if(Cam_Height < 0) {
                    Cam_Height = 0;
                } else if(Cam_Height > lim) {
                    Cam_Height = lim;
                }
            }
        }
    }
}

void GameScreen::ChangeActiveScreen(std::unique_ptr<Screen> NewScreen, std::string ID) {
    if(bHasActiveScreen() == false) {
        if(StateViewingScreen) {
            CloseScreenPreview();
        }
    }
    ScreenID = std::move(ID);
    ActiveScreen = std::move(NewScreen);
}

void GameScreen::CloseActiveScreen() {
    if(bHasActiveScreen() == true) {
        ActiveScreen.reset();
        ScreenID.clear();
    }
}

void GameScreen::CloseScreenPreview() {
    StateViewingScreen.reset();
    pin.reset();
}
