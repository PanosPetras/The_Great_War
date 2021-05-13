#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <string>
#include <functional>
#include <SDL_image.h>
#include <SDL_mixer.h>

class Screen;

class Button {
public:

    //The state of the button
    bool bHovered;

    //Stores the binded function
    std::function<void()> func;

    //Dimensions of the button
    SDL_Rect draw_rect;

    //Reference the the screen's renderer
    SDL_Renderer* RendererReference;

    //Saves the index of the keyboard key this button is binded to
    int key;

    //Reference to the button's texture
    SDL_Texture* texture;

    //Called when received input, to check whether the click was in this button
    void button_process_event(const SDL_Event* ev);

    //Constructor
    Button(SDL_Renderer* r, int x, int y, int Height, int Width, const char* image, std::function<void()> f = NULL, int keybind = NULL);

    //Destructor
    ~Button();

    //Render the buttons on the screen
    void RenderButton();

    //Change the image assigned to the button
    void ChangeImage(const char* image);

    //Change the button's position
    void ChangePosition(int x, int y, int Height, int Width);

    //Change the function binded to this button
    void ChangeFunctionBinding(std::function<void()> = NULL);

    //Reference to the path of the image assigned to the button
    std::string imagepath;
    std::string CurrentImage;

    //Update the properties of the button once something is changed
    void Update();

    void Playsound();

    Mix_Chunk* music = NULL;
};
#endif