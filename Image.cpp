#include "Image.h"

#include "MainWindow.h"

Image::Image(MainWindow& mw, std::string img, int x, int y, int Width, int Height, Anchor anchor) :
    Drawable(anchor), main_window(&mw), imagepath(img), draw_rect{.x = x, .y = y, .w = Width, .h = Height}, texture(main_window->IMG_Load(imagepath)) {
    ApplyAnchor(draw_rect, dAnchor);
}

void Image::pDraw() {
    // Drawing the Image
    SDL_RenderCopy(*main_window, texture, nullptr, &draw_rect);
}

void Image::ChangeImage(std::string img) {
    // Saving the new image path
    imagepath = std::move(img);
    Update();
}

void Image::ChangePosition(int x, int y, int Width, int Height) {
    // Saving the image's new coordinates
    draw_rect = {.x = x, .y = y, .w = Width, .h = Height};

    ApplyAnchor(draw_rect, dAnchor);
}

void Image::Update() {
    // Loading the image texture
    texture = main_window->IMG_Load(imagepath);
}
