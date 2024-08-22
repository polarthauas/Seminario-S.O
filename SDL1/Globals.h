#pragma once

struct SDL_Rect;

//Padrão 800x600

#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 50

namespace Global{
	enum RESIZE_MODE {
		RESIZE_MODE_WIDTH,
		RESIZE_MODE_HEIGHT
	};

	extern int windowWidth, windowHeight;
	extern int douglasWidth, douglasHeight;

	const int DESIGN_WIDTH = 1360;
	const int DESIGN_HEIGHT = 768;
	const int DESIGN_DOUGLAS_WIDTH = 32;
	const int DESIGN_DOUGLAS_HEIGHT = 32;

	int resizeValue(int a, RESIZE_MODE mode);

	bool clickedRect(int x, int y, SDL_Rect b);

	SDL_Rect resizeRect(SDL_Rect a);
}
