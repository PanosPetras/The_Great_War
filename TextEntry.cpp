#include "TextEntry.h"

#include "Image.h"
#include "Label.h"
#include "MainWindow.h"

#include <SDL.h>
TextEntry::TextEntry(MainWindow& mw, int X, int Y, int Width, int Height, std::string defaultText, int maxCharacters) : TextEntry(mw, X, Y, Width, Height, top_left, defaultText, maxCharacters) {
}

TextEntry::TextEntry(MainWindow& mw, int X, int Y, int Width, int Height, std::string defaultText, std::string Hint, int maxCharacters) : TextEntry(mw, X, Y, Width, Height, top_left, defaultText, Hint, maxCharacters) {
}

TextEntry::TextEntry(MainWindow& mw, int X, int Y, int Width, int Height, Anchor anchor, std::string defaultText, int maxCharacters) :
    main_window(&mw),
    x(X), y(Y),
    text(defaultText),
    maxSize(maxCharacters),
    background(std::make_unique<Image>(mw, "Backgrounds/FlagBg.png", x, y, Width, Height, anchor)),
    textLabel(std::make_unique<Label>(mw, defaultText, 20, int(x * 1.08), int(y * 1.08), anchor))
{
}

TextEntry::TextEntry(MainWindow& mw, int X, int Y, int Width, int Height, Anchor anchor, std::string defaultText, std::string Hint, int maxCharacters) :
    TextEntry(mw, X, Y, Width, Height, anchor, defaultText, maxCharacters)
{
    ChangeHint(Hint);
}

void TextEntry::HandleInput(const SDL_Event& ev){
    if (IsActive()) {
        if (ev.type == SDL_MOUSEBUTTONDOWN) {
            if (ev.button.x >= background->draw_rect.x &&
                ev.button.x <= (background->draw_rect.x + background->draw_rect.w) &&
                ev.button.y >= background->draw_rect.y &&
                ev.button.y <= (background->draw_rect.y + background->draw_rect.h)) {
                focused = true;
            }
            else {
                focused = false;
            }
        }

        if (focused) {
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym >= SDLK_a && ev.key.keysym.sym <= SDLK_z) {
                    text += char('a' + ev.key.keysym.sym - SDLK_a);

                    ChangeText(text);
                } else if (ev.key.keysym.sym >= SDLK_0 && ev.key.keysym.sym <= SDLK_9) {
                    text += char('0' + ev.key.keysym.sym - SDLK_0);

                    ChangeText(text);
                } else if (ev.key.keysym.sym == SDLK_BACKSPACE) {
                    if (text.size() > 0) {
                        text.pop_back();

                        ChangeText(text);
                    }
                }
            }
        }
    }
}

void TextEntry::ChangePosition(int X, int Y, int Width, int Height){
    background->ChangePosition(X, Y, Width, Height);
    textLabel->ChangePosition(int(X * 1.08), int(Y * 1.08));

    if (hintLabel) {
        hintLabel->ChangePosition(int(X * 1.08), int(Y * 1.08));
    }
}

bool TextEntry::IsFocused(){
    return focused;
}

std::string TextEntry::GetText(){
    return text;
}

void TextEntry::ChangeText(std::string Text){
    textLabel->ChangeText(Text);
    text = Text;
}

std::string TextEntry::GetHint(){
    return hint;
}

void TextEntry::ChangeHint(std::string Hint){
    if (Hint != hint) {
        if (hintLabel) {
            hintLabel->ChangeText(Hint);
        } else {
            hintLabel = std::make_unique<Label>(*main_window, Hint, 20, int(x * 1.08), int(y * 1.08));
        }

        hint = Hint;
    }
}

void TextEntry::pDraw(){
    background->Draw();

    if (text != "") {
        textLabel->Draw();
    } else if(hintLabel) {
        hintLabel->Draw();
    }
}
