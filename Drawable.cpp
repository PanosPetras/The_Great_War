#include "Drawable.h"
#include <SDL.h>

Drawable::Drawable(Anchor anchor) {
	Visibility = true;
	dAnchor = anchor;
}

void Drawable::Draw(){
	if (Visibility) {
		pDraw();
	}
}

InputDrawable::InputDrawable(Anchor anchor) : Drawable(anchor) {
}

void ApplyAnchor(SDL_Rect &rect, Anchor anchor) {
    switch (anchor) {
        case top_left:
            break;
        case top_right:
            rect.x -= rect.w;
            break;
        case bottom_left:
            rect.y -= rect.h;
            break;
        case bottom_right:
            rect.x -= rect.w;
            rect.y -= rect.h;
            break;
        case center:
            rect.x -= rect.w / 2;
            rect.y -= rect.h / 2;
            break;
    }
}
