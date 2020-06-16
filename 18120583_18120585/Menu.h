#ifndef _MENU_H
#define _MENU_H

/**
 *	MENU
 *	@author	Quoc Thuan Truong		18120583
 *	@author	Trieu Mai Ngoc Thuc		18120585
 *	@since 2019-10-26
 */

#include "FileUtils.h"
#include "file.h"
#include "LZSS.h"
#include <windows.h>
#include <conio.h>

class Menu {
private:
	const int FONT_SIZE = 16;
	const int WIDTH = 380;
	const int HEIGHT = 550;
private:
	int width;
	int height;
private:
	int option;
private:
	HANDLE consoleHandle;
private:
	void setFontSize(int size);
	void resizeConsole();
	void gotoXY(int x, int y);
	void putCharacterToConsole(char c, int x, int y);
	void putStringToConsole(const char str[], int x, int y);
private:
	void drawTitle(const char title[], int x, int y, bool haveFrame = false);
	void draw();
public:
	void run();
public:
	Menu();
};

#endif // !_MENU_H