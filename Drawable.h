#ifndef DRAWABLE_H
#define DRAWABLE_H

#define GLOBAL_FONT "Fonts/segoeui.ttf"

typedef union SDL_Event SDL_Event;
typedef struct SDL_Rect SDL_Rect;

typedef enum Anchor {
	top_left, top_right, bottom_left, bottom_right, center, center_top, center_bottom, center_left, center_right
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

	virtual void SetActive(bool state);
	bool GetActive();

protected:
	bool active;
};

void ApplyAnchor(SDL_Rect &rect, Anchor anchor);

#endif
