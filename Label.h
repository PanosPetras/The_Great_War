#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Label {
public:
    //The image's font
    TTF_Font* font;

    //The color of the text displayed by the label
    SDL_Color Color;

    //Dimensions of the label
    SDL_Rect draw_rect;

    //The texture containing the text surface
    SDL_Texture* texture;

    //The contents of the label
    std::string text;

    //Reference the the screen's renderer
    SDL_Renderer* RendererReference;

    //The label's position
    int Coords[2];

    //The label's font size
    int FontSize;

    //Constructor
    Label(SDL_Renderer* r, const char* Text, int size = 32, int x = 0, int y = 0, Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0);

    //Destructor
    ~Label();

    //Render the label on the screen
    void RenderLabel();

    //Change the label's text
    void ChangeText(const char* Text);

    //Change the label's color
    void ChangeColor(Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0);

    //Change the label's position
    void ChangePosition(int x, int y);

    //Update the properties of the label once something is changed
    void UpdateLabel();
};
#endif