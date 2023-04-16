#ifndef IMAGE_H
#define IMAGE_H

#include "Drawable.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class MainWindow;

class Image : public Drawable {
public:
	//Constructors
	Image(MainWindow& mw, std::string img, int x, int y, int Width, int Height, Anchor anchor = top_left);

	//Change the image assigned to the button
	void ChangeImage(std::string img);

	//Change the image's position
	void ChangePosition(int x, int y, int Height, int Width);

	//Update the image's properties once something's been modified
	void Update();

protected:
	//Reference the the main window
	MainWindow* main_window;

	//Render the image on the screen
	void pDraw();

	//Reference to the path of the image assigned to the button
	std::string imagepath;

	//Dimensions of the image
	SDL_Rect draw_rect;

	//The texture containing the image surface
	TextureRef texture;

	friend class TextEntry;
};
#endif
