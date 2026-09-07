#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "PlayerController.h"
#include "SDL_ColorDetection.h"
#include "ScreenList.h"
#include "UI.h"

#include <algorithm>
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

    if(bHasActiveScreen() == false) {
        HandleKeyboardPanning(elapsedMs);
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

int GameScreen::MaxCamHeight() const {
    return int((std::trunc(MapHeight * factor) - main_window->Height()) / factor);
}

void GameScreen::ClampCamHeight() {
    /*The world is a cylinder, so x wraps around and never runs out. y has
    nowhere to wrap to, so it stops at the poles.*/
    if(Cam_Height < 0) {
        Cam_Height = 0;
    } else if(Cam_Height > MaxCamHeight()) {
        Cam_Height = MaxCamHeight();
    }
}

void GameScreen::PanCamera(double dx, double dy) {
    PanRemainderX += dx;
    PanRemainderY += dy;

    // Whole map pixels move the camera; the fractions wait for the next call
    const int x = int(PanRemainderX), y = int(PanRemainderY);
    PanRemainderX -= x;
    PanRemainderY -= y;

    Cam_Width = WrapX(Cam_Width + x);
    Cam_Height += y;
    ClampCamHeight();
}

void GameScreen::Zoom(double steps) {
    /*Each step multiplies the magnification rather than adding to it, so a map
    zoomed in and back out by the same number of steps ends up where it began.*/
    const double previous = factor;
    factor = std::clamp(factor * std::pow(1 + ZoomingSpeed, steps), main_window->Width() / 3840.0, main_window->Width() / 480.0);

    if(factor == previous) {
        return;
    }

    /*Zooming changes how much of the world fits on the screen. Moving the
    camera by half of that change leaves the middle of the screen looking at
    the same place it was looking at before.*/
    const double centre = 1 / previous - 1 / factor;
    PanCamera(main_window->Width() / 2.0 * centre, main_window->Height() / 2.0 * centre);
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

        /*Dragging holds onto the map and pulls it along with the pointer, so
        the camera moves against the pointer rather than with it.*/
        if(mousepressed) {
            PanCamera(-MouseSensitivity * x / factor, -MouseSensitivity * y / factor);
        }

        // Scrolling either pans the map or changes its magnification
        if(ev.type == SDL_MOUSEWHEEL) {
            HandleScroll(ev.wheel);
        }

        /*Pinching to zoom. SDL builds this event out of finger events, so it
        arrives only from a device it counts as a touch device - a touchscreen,
        or a precision touchpad on Windows. A touchpad under Wayland is a
        pointer rather than a touch device and never produces it, which is what
        Ctrl and a scroll are for.*/
        if(ev.type == SDL_MULTIGESTURE && ev.mgesture.numFingers == 2) {
            Zoom(ev.mgesture.dDist * PinchingSpeed);
        }
    }
}

bool GameScreen::IsTrackpadScroll(const SDL_MouseWheelEvent& wheel) {
    /*A wheel turns in notches, so it reports whole steps, straight up or down.
    A trackpad reports a fraction of a step at a time, and reports sideways
    movement that a wheel has no way to produce.*/
    if(wheel.preciseX != 0 || wheel.preciseY != std::trunc(wheel.preciseY)) {
        LastTrackpadScrollMs = wheel.timestamp;
        return true;
    }

    /*Part way through a flick those numbers can land on a whole step and look
    like a wheel, which would zoom the map mid-pan. A device that was scrolling
    like a trackpad a moment ago is taken to still be one.*/
    return wheel.timestamp - LastTrackpadScrollMs < TrackpadScrollMemoryMs;
}

void GameScreen::HandleScroll(const SDL_MouseWheelEvent& wheel) {
    float x = wheel.preciseX, y = wheel.preciseY;

    // Some systems report a scroll the other way up, and say so
    if(wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
        x = -x;
        y = -y;
    }

    /*A trackpad has no middle button to drag the map with, so scrolling on one
    pans instead. A wheel keeps zooming, which is what it has always done and
    what it is good for. Ctrl zooms whichever device it is, so that zooming is
    still reachable when a device is mistaken for the other one.*/
    const bool trackpad = IsTrackpadScroll(wheel);
    if(trackpad && not(SDL_GetModState() & KMOD_CTRL)) {
        // The view follows the fingers: scrolling up looks further up the map
        PanCamera(x * ScrollingSpeed / factor, -y * ScrollingSpeed / factor);
    } else {
        Zoom(y);
    }
}

void GameScreen::HandleKeyboardPanning(Uint32 elapsedMs) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    const int x = keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT];
    const int y = keys[SDL_SCANCODE_DOWN] - keys[SDL_SCANCODE_UP];

    if(x == 0 && y == 0) {
        return;
    }

    /*The keys pan for as long as they are held, so how far the map moves is
    measured against how long the frame took rather than against the frame.*/
    const double distance = KeyboardSpeed * elapsedMs / factor;
    PanCamera(x * distance, y * distance);
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
