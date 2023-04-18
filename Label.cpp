#include "Label.h"
#include "MainWindow.h"

Label::Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, Uint8 red, Uint8 green, Uint8 blue) : Label(r, Text, size, x, y, 300, top_left, red, green, blue) {
}

Label::Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, Anchor anchor, Uint8 red, Uint8 green, Uint8 blue) : Label(r, Text, size, x, y, 300, anchor, red, green, blue) {
}

Label::Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, int xlim, Uint8 red, Uint8 green, Uint8 blue) : Label(r, Text, size, x, y, xlim, top_left, red, green, blue) {
}

Label::Label(SDL_Renderer_ctx& r, std::string Text, int size, int x, int y, int xlim, Anchor anchor, Uint8 red, Uint8 green, Uint8 blue) : Drawable(anchor), RendererReference(r), FontSize(size) {
    //Save the text assigned to the label in order to be used later
    text = Text;

    //Save the color assigned to the label in order to be used later
    Color = { red, green, blue, 0 };

    //Save the label's coordinates
    this->x = x, this->y = y, this->xLim = xlim;

    UpdateLabel();
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
    Color = { red, green, blue, 0 };
    UpdateLabel();
}

void Label::ChangePosition(int x, int y) {
    //Save the new Position
    this->x = x, this->y = y;

    //Setting the texture size
    int texW, texH;

    //Create the rectangle that will express the size of the texture we created
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    draw_rect = { x, y, texW, texH };

    ApplyAnchor(draw_rect, dAnchor);
}

void Label::ChangeXLimit(int xLim) {
    this->xLim = xLim;

    UpdateLabel();
}

void Label::UpdateLabel() {
    //Loading the font from the file
    TTF_Font_ctx font(FontSize);

    //Convert the text to a surface and then assign the surface to a texture
    auto surface = SDL_Surface_ctx::TTF_RenderText_Blended_Wrapped(font, text, Color, xLim);
    texture = SDL_Texture_ctx(RendererReference, surface);

    ChangePosition(x, y);
}

std::string Label::GetText(){
    return text;
}
