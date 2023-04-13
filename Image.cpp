#include "Image.h"

Image::Image(SDL_Renderer_ctx& r, std::string img, int x, int y, int Width, int Height, Anchor anchor) : Drawable(anchor), RendererReference(r) {
    ChangePosition(x, y, Width, Height);

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

void Image::pDraw(){
    //Drawing the Image
    SDL_RenderCopy(RendererReference, texture, NULL, &draw_rect);
}

void Image::ChangeImage(std::string img){
    //Saving the new image path
    imagepath = img;
    Update();
}

void Image::ChangePosition(int x, int y, int Width, int Height){
    //Saving the image's new coordinates
    draw_rect = { .x = x, .y = y, .w = Width, .h = Height };

    ApplyAnchor(draw_rect, dAnchor);
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
