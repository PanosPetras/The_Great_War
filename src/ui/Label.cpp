#include "ui/Label.h"

#include "core/MainWindow.h"

Label::Label(MainWindow& mw, std::string Text, FontSize size, int X, int Y, Color rgb) : Label(mw, std::move(Text), size, X, Y, 300, top_left, rgb) {}

Label::Label(MainWindow& mw, std::string Text, FontSize size, int X, int Y, Anchor anchor, Color rgb) : Label(mw, std::move(Text), size, X, Y, 300, anchor, rgb) {}

Label::Label(MainWindow& mw, std::string Text, FontSize size, int X, int Y, Uint32 xlim, Color rgb) : Label(mw, std::move(Text), size, X, Y, xlim, top_left, rgb) {}

Label::Label(MainWindow& mw, std::string Text, FontSize size, int X, int Y, Uint32 xlim, Anchor anchor, Color rgb) : Drawable(anchor), main_window(&mw), fontSize(size), color(rgb), text(std::move(Text)), x(X), y(Y), xLim(xlim) {
    UpdateLabel();
}

void Label::pDraw() {
    // Copy the text texture to the screen
    RenderTexture(*main_window, texture, draw_rect);
}

void Label::ChangeText(std::string_view Text) {
    /*Re-rendering costs a glyph raster and a texture upload, and screens call
    this every frame with a value that only changes once per in-game day. The
    parameter is a view rather than a string so that the overwhelmingly common
    call - the one that turns out to change nothing - does not allocate just to
    build the value it is about to compare and throw away.*/
    if(Text == text) return;

    // Assign the new text to the label
    text = Text;
    UpdateLabel();
}

void Label::ChangeTextSize(FontSize size) {
    if(size == fontSize) return;

    fontSize = size;
    UpdateLabel();
}

void Label::ChangeColor(Color rgb) {
    const SDL_Color sc(rgb);
    if(sc.r == color.r && sc.g == color.g && sc.b == color.b && sc.a == color.a) return;

    // Assign the new color to the label
    color = sc;
    UpdateLabel();
}

void Label::ChangePosition(int X, int Y) {
    // Save the new Position
    x = X;
    y = Y;

    draw_rect = {x, y, texW, texH};

    ApplyAnchor(draw_rect, dAnchor);
}

void Label::ChangeXLimit(Uint32 xlim) {
    if(xlim == xLim) return;

    xLim = xlim;
    UpdateLabel();
}

void Label::UpdateLabel() {
    // The window keeps every font open, so this does not touch the disk
    FontRef font = main_window->TTF_OpenFont(fontSize);

    // Convert the text to a surface and then assign the surface to a texture
    auto surface = SDL_Surface_ctx::TTF_RenderText_Blended_Wrapped(font, text, color, xLim);

    /*The surface already knows how big the glyph run it just rasterised is,
    and a rendered run is a whole number of pixels, so take the size from it
    rather than asking SDL for the texture's size back in floats.*/
    texW = surface->w;
    texH = surface->h;

    texture = SDL_Texture_ctx(*main_window, surface);

    ChangePosition(x, y);
}

const std::string& Label::GetText() const {
    return text;
}
