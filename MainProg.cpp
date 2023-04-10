#define SDL_MAIN_HANDLED
#include "MainWindow.h"

int main() {
    MainWindow::Instance().MainLoop();
}
