#include "Label.h"

Label::Label(SDL_Renderer* r, std::string Text, int size, int x, int y, Uint8 red, Uint8 green, Uint8 blue) : Label(r, Text, size, x, y, top_left, red, green, blue) {
}

Label::Label(SDL_Renderer* r, std::string Text, int size, int x, int y, Anchor anchor, Uint8 red, Uint8 green, Uint8 blue) : Drawable(anchor) {
    //Save the text assigned to the label in order to be used later
    text = Text;

    //Saving the renderer's reference
    RendererReference = r;

    //Save the color assigned to the label in order to be used later
    Color = { red, green, blue };

    //Save the label's coordinates
    Coords[0] = x;
    Coords[1] = y;

    //Save the label's font size
    FontSize = size;

    //Loading the font from the file
    TTF_Font* font = TTF_OpenFont(GLOBAL_FONT, FontSize);

    //Convert the text to a surface
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), Color);

    //Create the texture out of the surface that we just generated
    texture = SDL_CreateTextureFromSurface(RendererReference, surface);

    //Free the surface
    SDL_FreeSurface(surface);

    //Delete the font
    TTF_CloseFont(font);

    //Setting the texture size
    int texW = 0;
    int texH = 0;

    //Create the rectangle that will express the size of the texture we created
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    draw_rect = { Coords[0], Coords[1], texW, texH };
}

Label::~Label(){
    //Free up the memory
    SDL_DestroyTexture(texture);
}

void Label::pDraw() {
    //Copy the text texture to the screen
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);
}

void Label::ChangeText(std::string Text) {
    //Assign the new text to the label
    text = Text;
    UpdateLabel();
}

void Label::ChangeTextSize(int size){
    FontSize = size;
    UpdateLabel();
}

void Label::ChangeColor(Uint8 red, Uint8 green, Uint8 blue) {
    //Assign the new color to the label
    Color = { red, green, blue };
    UpdateLabel();
}

void Label::ChangePosition(int x, int y) {
    //Change the label's coordinates
    Coords[0] = x;
    Coords[1] = y;
    UpdateLabel();
}

void Label::UpdateLabel() {
    //Free up the memory
    SDL_DestroyTexture(texture);

    //Loading the font from the file
    TTF_Font* font = TTF_OpenFont(GLOBAL_FONT, FontSize);

    //Convert the text to a surface and then assign the surface to a texture
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), Color);
    texture = SDL_CreateTextureFromSurface(RendererReference, surface);
    SDL_FreeSurface(surface);

    //Delete the font
    TTF_CloseFont(font);

    //Setting the texture size
    int texW = 0;
    int texH = 0;

    //Create the rectangle that will express the size of the texture we created
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    draw_rect = { Coords[0], Coords[1], texW, texH };

    switch (dAnchor) {
    case top_left:
        break;
    case top_right:
        draw_rect.x -= texW;
        break;
    case bottom_left:
        draw_rect.y -= texH;
        break;
    case bottom_right:
        draw_rect.x -= texW;
        draw_rect.y -= texH;
        break;
    case center:
        draw_rect.x -= texW / 2;
        draw_rect.y -= texH / 2;
        break;
    }
}

std::string Label::GetText(){
    return text;
}
