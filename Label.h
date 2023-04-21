#ifndef LABEL_H
#define LABEL_H

#include "Drawable.h"

#include "Color.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

class MainWindow;

class Label : public Drawable {
public:
    // Constructor
    Label(MainWindow& mw, std::string Text, int size, int x, int y, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, int size, int x, int y, Anchor anchor, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, int size, int x, int y, Uint32 xLim, Color rgb = Color{});
    Label(MainWindow& mw, std::string Text, int size, int x, int y, Uint32 xLim, Anchor anchor, Color rgb = Color{});

    // Get the label's text
    std::string GetText();

    // Change the label's text
    void ChangeText(std::string Text);

    /// Change the size of the label's text
    void ChangeTextSize(int size);

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
    int FontSize;

    // The color of the text displayed by the label
    SDL_Color color;

    // Dimensions of the label
    SDL_Rect draw_rect;

    // The texture containing the text surface
    SDL_Texture_ctx texture;

    // The contents of the label
    std::string text;

    // The label's position
    int x, y;

    // The label's pixel limit in width before it enters a new line
    Uint32 xLim;
};
#endif
