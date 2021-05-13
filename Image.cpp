#include "Image.h"

Image::Image(SDL_Renderer* r, const char* img, int x, int y, int Width, int Height){
    ChangePosition(x, y, Width, Height);
    
    //Saving the renderer's reference
    RendererReference = r;

    //Saving the image path
    imagepath = img;

    //Loading the image texture
    image = IMG_Load(imagepath.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, image);
}

Image::~Image(){
    //Free up the memory
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
}

void Image::RenderImage(){
    //Drawing the Image
    SDL_RenderCopy(RendererReference, texture, NULL, &this->draw_rect);
}

void Image::ChangeImage(const char* img){
    //Saving the new image path
    imagepath = img;
    Update();
}

void Image::ChangePosition(int p, int d, int Height, int Width){
    //Saving the image's new coordinates
    this->draw_rect.x = p;
    this->draw_rect.y = d;
    this->draw_rect.w = Height;
    this->draw_rect.h = Width;
}

void Image::Update(){
    //Free up the memory
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);

    //Loading the image texture
    image = IMG_Load(imagepath.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, image);
    //SDL_SetTextureColorMod(texture, 255, 255, 255); //in order to change color of the state
}
