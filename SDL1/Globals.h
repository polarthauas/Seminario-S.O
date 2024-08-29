#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>
#include <string>

//Padrão 800x600

#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 50

namespace Global {
	enum RESIZE_MODE {
		RESIZE_MODE_WIDTH,
		RESIZE_MODE_HEIGHT
	};

	extern int windowWidth, windowHeight;
	extern int douglasWidth, douglasHeight;

	const int DESIGN_WIDTH = 1360;
	const int DESIGN_HEIGHT = 768;
	const int DESIGN_DOUGLAS_WIDTH = 64;
	const int DESIGN_DOUGLAS_HEIGHT = 64;

	int resizeValue(int a, RESIZE_MODE mode);

	bool mouseInRect(int x, int y, SDL_Rect& b);

	int getTextWidth(const std::string& text, TTF_Font* font);

	int getTextHeight(const std::string& text, const std::string& fontPath, int fontSize);

	SDL_Rect resizeRect(SDL_Rect a);

}
