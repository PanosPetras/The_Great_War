#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Image {
public:
	//Reference to the path of the image assigned to the button
	std::string imagepath;

	//Dimensions of the image
	SDL_Rect draw_rect;

	//Reference the the screen's renderer
	SDL_Renderer* RendererReference;

	//The surface that will be created out of the image
	SDL_Surface* image;

	//The texture containing the image surface
	SDL_Texture* texture;

	//Constructor
	Image(SDL_Renderer* r, const char* img, int x, int y, int Width, int Height);

	//Destructor
	~Image();

	//Render the image on the screen
	void RenderImage();

	//Change the image assigned to the button
	void ChangeImage(const char* img);

	//Change the image's position
	void ChangePosition(int p, int d, int Height, int Width);

	//Update the image's properties once something's been modified
	void Update();
};
#endif