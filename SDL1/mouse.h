#pragma once

#include <SDL.h>

class Mouse
{
public:	
	Mouse(SDL_Renderer* rend);

	void moveMouseSmoothly(int targetX, int targetY, int durationMs);

};

