#include "ProgressCircle.h"

ProgressCircle::ProgressCircle(SDL_Renderer* r, int x, int y, int Height, int Width){
	RendererReference = r;
	ChangePosition(x, y);
	ChangeSize(Height, Width);

	CurrentFrame = 0;

	bIsActive = true;

	WindowSize[0] = Width;
	WindowSize[1] = Height;

	//Load all the frames as textures
	SDL_Surface* img;
	std::string path;
	for (int i = 0; i < 27; i++) {
		path = "Loading Widget/Frames " + std::to_string(i) + ".png";
		img = IMG_Load(path.c_str());

		Frames[i] = SDL_CreateTextureFromSurface(r, img);

		SDL_FreeSurface(img);
	}

	//Initiate the frame cycling thread
	thread = SDL_CreateThread(&ProgressCircle::AnimationCycle, NULL, this);
}

ProgressCircle::~ProgressCircle(){
	//Delete all the Frame textures
	for (int x = 0; x < 27; x++) {
		SDL_DestroyTexture(Frames[x]);
	}

	//Wait for the frame cycle thread to finish
	if (bIsActive == true) {
		bIsActive = false;
		SDL_WaitThread(thread, nullptr);
	}
}

void ProgressCircle::Render(){
	//Drawing the ProgressCircle
	SDL_RenderCopy(RendererReference, Frames[CurrentFrame], NULL, &draw_rect); 
}

void ProgressCircle::ChangePosition(int x, int y){
	this->draw_rect.x = x;
	this->draw_rect.y = y;
}

void ProgressCircle::ChangeSize(int Height, int Width){
	this->draw_rect.w = Width;
	this->draw_rect.h = Height;
}

int ProgressCircle::AnimationCycle(void* data){
	ProgressCircle* Reference = static_cast<ProgressCircle*>(data);
	while (Reference->bIsActive) {
		//50 ms delay
		int x = 0;
		while (x < 2 && Reference-> bIsActive == true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			x++;
		}

		/*Increment the Current frame by one. 
		If it surpasses the frame limit, it goes back to 0*/
		Reference->CurrentFrame++;
		if (Reference->CurrentFrame >= 27) {
			Reference->CurrentFrame = 0;
		}
	}

	return 0;
}