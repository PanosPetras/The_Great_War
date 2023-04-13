#include "Image.h"

Image::Image(SDL_Renderer_ctx& r, std::string img, int x, int y, int Width, int Height, Anchor anchor) :
    Drawable(anchor),
    RendererReference(r),
    imagepath(img),
    draw_rect{.x = x, .y = y, .w = Width, .h = Height},
    texture(SDL_Texture_ctx::IMG_Load(RendererReference, imagepath))
{
    ApplyAnchor(draw_rect, dAnchor);
}

void Image::pDraw(){
    //Drawing the Image
    SDL_RenderCopy(RendererReference, texture, nullptr, &draw_rect);
}

void Image::ChangeImage(std::string img){
    //Saving the new image path
    imagepath = std::move(img);
    Update();
}

void Image::ChangePosition(int x, int y, int Width, int Height){
    //Saving the image's new coordinates
    draw_rect = { .x = x, .y = y, .w = Width, .h = Height };

    ApplyAnchor(draw_rect, dAnchor);
}

void Image::Update(){
    //Loading the image texture
    texture = SDL_Texture_ctx::IMG_Load(RendererReference, imagepath);
    //SDL_SetTextureColorMod(texture, 255, 255, 255); //in order to change color of the state
}
