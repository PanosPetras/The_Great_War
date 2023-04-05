#include "Image.h"

Image::Image(SDL_Renderer* r, std::string img, int x, int y, int Width, int Height, Anchor anchor) : Drawable(anchor) {
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
    draw_rect.x = x;
    draw_rect.y = y;
    draw_rect.w = Width;
    draw_rect.h = Height;

    switch (dAnchor) {
    case top_left:
        break;
    case top_right:
        draw_rect.x -= Width;
        break;
    case bottom_left:
        draw_rect.y -= Height;
        break;
    case bottom_right:
        draw_rect.x -= Width;
        draw_rect.y -= Height;
        break;
    case center:
        draw_rect.x -= Width / 2;
        draw_rect.y -= Height / 2;
        break;
    }
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
