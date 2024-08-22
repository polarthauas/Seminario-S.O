#include "Globals.h"
#include <iostream>
#include <SDL2/SDL_rect.h>

int Global::windowHeight;
int Global::windowWidth;

int Global::douglasHeight;
int Global::douglasWidth;

int Global::resizeValue(int v, Global::RESIZE_MODE mode)
{
	float scaleX = (windowWidth / (DESIGN_WIDTH*1.0f));
	float scaleY = (windowHeight / (DESIGN_HEIGHT * 1.0f));

	switch (mode)
	{
	case Global::RESIZE_MODE_WIDTH:
		return static_cast<int>(v * scaleX);
	case Global::RESIZE_MODE_HEIGHT:
		return static_cast<int>(v * scaleY);
	default:
		break;
	}
}

bool Global::clickedRect(int x, int y, SDL_Rect b) {
	return (x >= b.x && x <= b.x + b.w &&
		y >= b.y && y <= b.y + b.h);
}

SDL_Rect Global::resizeRect(SDL_Rect a)
{
	SDL_Rect _rect = {};
	_rect.x = resizeValue(a.x, RESIZE_MODE_WIDTH);
	_rect.y = resizeValue(a.y, RESIZE_MODE_HEIGHT);
	_rect.w = resizeValue(a.w, RESIZE_MODE_WIDTH);
	_rect.h = resizeValue(a.h, RESIZE_MODE_HEIGHT);

	return _rect;
}
