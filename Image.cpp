#include "Image.h"

Image::Image(SDL_Renderer_ctx& r, std::string img, int x, int y, int Width, int Height, Anchor anchor) : Drawable(anchor), RendererReference(r) {
    ChangePosition(x, y, Width, Height);

    //Saving the image path
    imagepath = img;

    //Loading the image texture
    SDL_Surface_ctx image(IMG_Load(imagepath.c_str()));

    texture = SDL_Texture_ctx(RendererReference, image);
}

void Image::pDraw(){
    //Drawing the Image
    SDL_RenderCopy(RendererReference, texture, nullptr, &draw_rect);
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
    SDL_Surface_ctx image(IMG_Load(imagepath.c_str()));
    texture = SDL_Texture_ctx(RendererReference, image);
    //SDL_SetTextureColorMod(texture, 255, 255, 255); //in order to change color of the state
}
