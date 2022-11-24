#ifndef DRAWABLE_H
#define DRAWABLE_H

#define GLOBAL_FONT "Fonts/segoeui.ttf"

typedef union SDL_Event SDL_Event;

class Drawable {
public:
	Drawable();

	void Draw();

	bool Visibility;

protected:
	virtual void pDraw() = 0;
};

class InputDrawable : public Drawable {
public:
	virtual void HandleInput(const SDL_Event*) = 0;
};

#endif
