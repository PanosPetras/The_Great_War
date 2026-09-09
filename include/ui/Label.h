#ifndef LABEL_H
#define LABEL_H

#include "ui/Drawable.h"

#include "core/Color.h"
#include "core/FontSize.h"

#include "core/SDL_ctx.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>

class MainWindow;

class Label : public Drawable {
public:
    // Constructor
    Label(MainWindow& mw, std::string Text, FontSize size, int x, int y, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, FontSize size, int x, int y, Anchor anchor, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, FontSize size, int x, int y, Uint32 xLim, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, FontSize size, int x, int y, Uint32 xLim, Anchor anchor, Color rgb = Color{});

    // Get the label's text
    const std::string& GetText() const;

    // Change the label's text
    void ChangeText(std::string_view Text);

    /// Change the size of the label's text
    void ChangeTextSize(FontSize size);

    // Change the label's color
    void ChangeColor(Color rgb);

    // Change the label's position
    void ChangePosition(int x, int y);

    // Change the label's x limit
    void ChangeXLimit(Uint32 xLim);

    // Update the properties of the label once something is changed
    void UpdateLabel();

protected:
    // Reference to the main window
    MainWindow* main_window;

    // Render the label on the screen
    void pDraw() override;

    // The label's font size
    FontSize fontSize;

    // The color of the text displayed by the label
    SDL_Color color;

    // Dimensions of the label
    SDL_Rect draw_rect;

    // The texture containing the text surface
    SDL_Texture_ctx texture;

    // The contents of the label
    std::string text;

    /*The size of the rendered glyph run, kept from the surface it was
    rasterised into so that the layout never has to ask SDL for it again.*/
    int texW = 0, texH = 0;

    // The label's position
    int x, y;

    // The label's pixel limit in width before it enters a new line
    Uint32 xLim;
};
#endif
