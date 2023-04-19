#ifndef SCREEN_H
#define SCREEN_H

#include "Drawable.h"
#include "Image.h"
#include "Label.h"
#include "MessageBox.h"

#include "SDL_ctx.h"

#include <SDL.h>
#include <SDL_image.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class MainWindow;

class Screen {
public:
    //Constructor
    Screen(MainWindow& mw);
    Screen(MainWindow& mw, std::function<void()> qf, std::function<void(std::unique_ptr<Screen>)> csf = {});

    virtual ~Screen() = default;

    //Renders all of the screen's components
    virtual void Render();

    //Handles input events
    virtual void Handle_Input(SDL_Event& ev);

    void DeleteMessageBox(void* p);

protected:
    //This is a reference to the main window
    MainWindow* main_window;

    //Allows the creation of Images on the screen
    std::vector<std::unique_ptr<Image>> ImageArr;

    template<class T, class... Args>
    void AddImage(Args&&... args) {
        ImageArr.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    //Allows the creation of Drawable elements on the screen
    inline std::size_t InputDrawableArrtop() const { return InputDrawableArr.size(); }

    template<class T, class... Args>
    void AddDrawable(Args&&... args) {
        InputDrawableArr.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<class T>
    T& As(std::size_t idx) {
        return *static_cast<T*>(InputDrawableArr[idx].get());
    }

    //Allows the creation of Labels on the screen
    std::vector<std::unique_ptr<Label>> LabelArr;

    template<class T, class... Args>
    void AddLabel(Args&&... args) {
        LabelArr.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<class T, class... Args>
    void AddMessageBox(Args&&... args) {
        messageBoxes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    bool bHasBackground = false;

    //Stores the image's texture
    SDL_Texture_ctx texture;

    std::function<void()> QuitFunc;
    std::function<void(std::unique_ptr<Screen>)> ChangeScreenFunc;

    //Sets the screen's background
    virtual void RenderBackground();

    void SetupBg(const char* bg);

    std::vector<std::unique_ptr<InputDrawable>> InputDrawableArr;

private:
    std::vector<std::unique_ptr<MessageBox>> messageBoxes;

};
#endif
