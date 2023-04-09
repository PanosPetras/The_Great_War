#include "WindowInfo.h"

int WindowInfo::height = 0, WindowInfo::width = 0;

int WindowInfo::GetWindowWidth() {
    return width;
}

int WindowInfo::GetWindowHeight() {
    return height;
}

int (*GetWindowWidth)() = WindowInfo::GetWindowWidth;
int (*GetWindowHeight)() = WindowInfo::GetWindowHeight;
