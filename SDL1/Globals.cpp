#include "Globals.h"
#include <SDL_image.h>
#include <iostream>

int windowHeight;
int windowWidth;

int calcAlterWindowSize(int v, char mode)
{
	float scaleX = (windowWidth / (DESIGN_WIDTH*1.0f));
	float scaleY = (windowHeight / (DESIGN_HEIGHT * 1.0f));

	if (mode == 'w') return static_cast<int>(v * scaleX);
	else if (mode == 'h') return static_cast<int>(v * scaleY);
}

bool clickedRect(int x, int y, SDL_Rect b) {
	return (x >= b.x && x <= b.x + b.w &&
		y >= b.y && y <= b.y + b.h);
}
