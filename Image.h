#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Drawable.h"

class Image : public Drawable {
public:
	//Constructors
	Image(SDL_Renderer* r, std::string img, int x, int y, int Width, int Height, Anchor anchor = top_left);

	//Destructor
	~Image();

	//Change the image assigned to the button
	void ChangeImage(std::string img);

	//Change the image's position
	void ChangePosition(int x, int y, int Height, int Width);

	//Update the image's properties once something's been modified
	void Update();

protected:
	//Render the image on the screen
	void pDraw();

	//Reference to the path of the image assigned to the button
	std::string imagepath;

	//Dimensions of the image
	SDL_Rect draw_rect;

	//Reference the the screen's renderer
	SDL_Renderer* RendererReference;

	//The texture containing the image surface
	SDL_Texture* texture;

	friend class TextEntry;
};
#endif