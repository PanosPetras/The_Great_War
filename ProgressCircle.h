#ifndef ProgressCircle_H
#define ProgressCircle_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <chrono>
#include <thread>

class ProgressCircle{
public:
	//Constructor
	ProgressCircle(SDL_Renderer* r, int x, int y, int Height, int Width);

	//Destructor
	~ProgressCircle();

	//Render the ProgressCircle
	void Render();

	//Change the Circle's location
	void ChangePosition(int x, int y);

	//Change the Circle's size
	void ChangeSize(int Height, int Width);
	
	//The animation cycling, in other words, it switches the frames
	static int AnimationCycle(void* data);

	//The thread that will handle the animation cycling
	SDL_Thread* thread;

	//A reference to the window's renderer
	SDL_Renderer* RendererReference;

	//Dimensions of the Circle
	SDL_Rect draw_rect;

	//The gif's frames
	SDL_Texture* Frames[27];

	//The Window's dimensions
	int WindowSize[2];

	//The current frame being displayed to the screen
	int CurrentFrame;

	//Whether it is active or not
	bool bIsActive;
};
#endif