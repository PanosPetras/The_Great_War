#ifndef DRAWABLE_H
#define DRAWABLE_H

#define GLOBAL_FONT "Fonts/segoeui.ttf"

typedef union SDL_Event SDL_Event;

typedef enum Anchor {
	top_left, top_right, bottom_left, bottom_right, center
} Anchor;

class Drawable {
public:
	Drawable(Anchor anchor = top_left);

	void Draw();

	bool Visibility;

protected:
	Anchor dAnchor;

	virtual void pDraw() = 0;
};

class InputDrawable : public Drawable {
public:
	InputDrawable(Anchor anchor = top_left);

	virtual void HandleInput(const SDL_Event*) = 0;
};

#endif
