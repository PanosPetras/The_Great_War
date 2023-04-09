#include <SDL.h>
#include "TextEntry.h"
#include "Image.h"
#include "Label.h"

TextEntry::TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText, int maxCharacters) : TextEntry(r, x, y, Width, Height, top_left, defaultText, maxCharacters) {
}

TextEntry::TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, std::string defaultText, std::string hint, int maxCharacters) : TextEntry(r, x, y, Width, Height, top_left, defaultText, hint, maxCharacters) {
}

TextEntry::TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, Anchor anchor, std::string defaultText, int maxCharacters) {
    background = std::make_unique<Image>(r, "Backgrounds/FlagBg.png", x, y, Width, Height, anchor);
    textLabel = std::make_unique<Label>(r, defaultText, 20, int(x * 1.08), int(y * 1.08), anchor);
    hintLabel = NULL;

    text = defaultText;
    hint = "";
    maxSize = maxCharacters;

    this->x = x;
    this->y = y;
}

TextEntry::TextEntry(SDL_Renderer* r, int x, int y, int Width, int Height, Anchor anchor, std::string defaultText, std::string hint, int maxCharacters) : TextEntry(r, x, y, Width, Height, anchor, defaultText, maxCharacters) {
    ChangeHint(hint);
}

void TextEntry::HandleInput(const SDL_Event& ev){
    if (active) {
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

void TextEntry::ChangePosition(int x, int y, int Width, int Height){
    background->ChangePosition(x, y, Width, Height);
    textLabel->ChangePosition(int(x * 1.08), int(y * 1.08));

    if (hintLabel != NULL) {
        hintLabel->ChangePosition(int(x * 1.08), int(y * 1.08));
    }
}

bool TextEntry::IsFocused(){
    return focused;
}

std::string TextEntry::GetText(){
    return text;
}

void TextEntry::ChangeText(std::string text){
    textLabel->ChangeText(text);
    this->text = text;
}

std::string TextEntry::GetHint(){
    return hint;
}

void TextEntry::ChangeHint(std::string hint){
    if (hint != this->hint) {
        if (hintLabel != NULL) {
            hintLabel->ChangeText(hint);
        } else {
            hintLabel = std::make_unique<Label>(RendererReference, hint, 20, int(x * 1.08), int(y * 1.08));
        }

        this->hint = hint;
    }
}

void TextEntry::pDraw(){
    background->Draw();

    if (text != "") {
        textLabel->Draw();
    } else if(hintLabel != NULL) {
        hintLabel->Draw();
    }
}
