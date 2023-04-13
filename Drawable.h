#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SDL.h>

#define GLOBAL_FONT "Fonts/segoeui.ttf"

enum Anchor {
	top_left, top_right, bottom_left, bottom_right, center, center_top, center_bottom, center_left, center_right
};

class Drawable {
public:
	Drawable(Anchor anchor = top_left);
        virtual ~Drawable() = default;

	void Draw();

	bool Visibility;

protected:
	Anchor dAnchor;

	virtual void pDraw() = 0;
};

class InputDrawable : public Drawable {
public:
	InputDrawable(Anchor anchor = top_left);

	virtual void HandleInput(const SDL_Event&) = 0;

	virtual void SetActive(bool state);
	bool GetActive();

protected:
	bool active;
};

void ApplyAnchor(SDL_Rect& rect, Anchor anchor);

#endif
