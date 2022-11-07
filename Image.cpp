#include "Image.h"

Image::Image(SDL_Renderer* r, const char* img, int x, int y, int Width, int Height){
    ChangePosition(x, y, Width, Height);
    
    //Saving the renderer's reference
    RendererReference = r;

    //Saving the image path
    imagepath = img;

    //Loading the image texture
    SDL_Surface* image = IMG_Load(imagepath.c_str());

    texture = SDL_CreateTextureFromSurface(RendererReference, image);

    SDL_FreeSurface(image);
}

Image::~Image(){
    //Free up the memory
    SDL_DestroyTexture(texture);
}

void Image::RenderImage(){
    //Drawing the Image
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);
}

void Image::ChangeImage(const char* img){
    //Saving the new image path
    imagepath = img;
    Update();
}

void Image::ChangePosition(int p, int d, int Height, int Width){
    //Saving the image's new coordinates
    draw_rect.x = p;
    draw_rect.y = d;
    draw_rect.w = Height;
    draw_rect.h = Width;
}

void Image::Update(){
    //Free up the memory
    SDL_DestroyTexture(texture);

    //Loading the image texture
    SDL_Surface* image = IMG_Load(imagepath.c_str());
    texture = SDL_CreateTextureFromSurface(RendererReference, image);
    SDL_FreeSurface(image);
    //SDL_SetTextureColorMod(texture, 255, 255, 255); //in order to change color of the state
}
