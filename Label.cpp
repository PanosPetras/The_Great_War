#include "Label.h"

#include "MainWindow.h"

Label::Label(MainWindow& mw, std::string Text, int size, int X, int Y, Color rgb) :
    Label(mw, Text, size, X, Y, 300, top_left, rgb) {}

Label::Label(MainWindow& mw, std::string Text, int size, int X, int Y, Anchor anchor, Color rgb) :
    Label(mw, Text, size, X, Y, 300, anchor, rgb) {}

Label::Label(MainWindow& mw, std::string Text, int size, int X, int Y, Uint32 xlim, Color rgb) :
    Label(mw, Text, size, X, Y, xlim, top_left, rgb) {}

Label::Label(MainWindow& mw, std::string Text, int size, int X, int Y, Uint32 xlim, Anchor anchor, Color rgb) :
    Drawable(anchor), main_window(&mw), FontSize(size), color(rgb), text(Text), x(X), y(Y), xLim(xlim) {
    UpdateLabel();
}

void Label::pDraw() {
    // Copy the text texture to the screen
    SDL_RenderCopy(*main_window, texture, nullptr, &draw_rect);
}

void Label::ChangeText(std::string Text) {
    // Assign the new text to the label
    text = Text;
    UpdateLabel();
}

void Label::ChangeTextSize(int size) {
    FontSize = size;
    UpdateLabel();
}

void Label::ChangeColor(Color rgb) {
    // Assign the new color to the label
    color = SDL_Color(rgb);
    UpdateLabel();
}

void Label::ChangePosition(int X, int Y) {
    // Save the new Position
    x = X;
    y = Y;

    // Setting the texture size
    int texW, texH;

    // Create the rectangle that will express the size of the texture we created
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    draw_rect = {x, y, texW, texH};

    ApplyAnchor(draw_rect, dAnchor);
}

void Label::ChangeXLimit(Uint32 xlim) {
    xLim = xlim;
    UpdateLabel();
}

void Label::UpdateLabel() {
    // Loading the font from the file
    TTF_Font_ctx font(FontSize);

    // Convert the text to a surface and then assign the surface to a texture
    auto surface = SDL_Surface_ctx::TTF_RenderText_Blended_Wrapped(font, text, color, xLim);
    texture = SDL_Texture_ctx(*main_window, surface);

    ChangePosition(x, y);
}

std::string Label::GetText() {
    return text;
}
