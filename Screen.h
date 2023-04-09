#ifndef SCREEN_H
#define SCREEN_H

#include "Drawable.h"
#include "Image.h"
#include "Label.h"

#include <SDL.h>
#include <SDL_image.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class MessageBox;

class Screen{
public:
	//Constructor
	Screen(SDL_Renderer* r);

	//Destructor
	virtual ~Screen();

	//Renders all of the screen's components
	virtual void Render();

	//Handles input events
	virtual void Handle_Input(SDL_Event* ev);

	void DeleteMessageBox(void* p);

protected:
	//This is a reference to the main window's renderer
	SDL_Renderer* renderer;

	//Allows the creation of Images on the screen
        std::vector<std::unique_ptr<Image>> ImageArr;
        inline std::size_t ImageArrtop() const { return ImageArr.size(); }

        template<class T, class... Args>
        void AddImage(Args&&... args) {
            ImageArr.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

	//Allows the creation of Drawable elements on the screen
        std::vector<std::unique_ptr<InputDrawable>> InputDrawableArr;
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
        inline std::size_t LabelArrtop() const { return LabelArr.size(); }

        template<class T, class... Args>
        void AddLabel(Args&&... args) {
            LabelArr.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

	std::vector<MessageBox*> messageBoxes;
	
	bool bHasBackground;

	//Stores the image's texture
	SDL_Texture* texture;

	std::function<void(Screen*)> ChangeScreenFunc;
	std::function<void()> QuitFunc;

	//Sets the screen's background
	virtual void RenderBackground();
	
	void SetupBg(const char* bg);
};
#endif
