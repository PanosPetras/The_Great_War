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

extern int(*GetWindowWidth)();
extern int(*GetWindowHeight)();

#endif
