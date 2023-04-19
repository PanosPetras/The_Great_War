#ifndef LABEL_H
#define LABEL_H

#include "Drawable.h"

#include "Color.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <string>

class Label : public Drawable {
public:
    //Constructor
    Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, Color rgb = Color{});
    Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, Anchor anchor, Color rgb = Color{});
    Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, int xLim, Color rgb = Color{});
    Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, int xLim, Anchor anchor, Color rgb = Color{});

    //Get the label's text
    std::string GetText();

    //Change the label's text
    void ChangeText(std::string Text);

    ///Change the size of the label's text
    void ChangeTextSize(int size);

    //Change the label's color
    void ChangeColor(Color rgb);

    //Change the label's position
    void ChangePosition(int x, int y);

    //Change the label's x limit
    void ChangeXLimit(int xLim);

    //Update the properties of the label once something is changed
    void UpdateLabel();

protected:
    //Render the label on the screen
    void pDraw() override;

    //Reference the the screen's renderer
    RendererRef RendererReference;

    //The label's font size
    int FontSize;

    //The color of the text displayed by the label
    SDL_Color color;

    //Dimensions of the label
    SDL_Rect draw_rect;

    //The texture containing the text surface
    SDL_Texture_ctx texture;

    //The contents of the label
    std::string text;

    //The label's position
    int x, y;

    //The label's pixel limit in width before it enters a new line
    int xLim;
};
#endif
