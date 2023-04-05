#ifndef WINDOWINFO_H
#define WINDOWINFO_H

class WindowInfo {
public:
	static int GetWindowWidth();
	static int GetWindowHeight();

private:
	static int width, height;

	friend class MainWindow;
};

static auto GetWindowWidth = WindowInfo::GetWindowWidth;
static auto GetWindowHeight = WindowInfo::GetWindowHeight;

#endif
