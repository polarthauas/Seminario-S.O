#pragma once

#include "globals.h"

#include <string>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

class MessageManager
{
public:
	MessageManager();
	~MessageManager();

	[[nodiscard]] bool setFont(const std::string& fontPath, int fontSize);

	void Render(SDL_Renderer* rend, const std::string& message, SDL_Color color, int x, int y, bool square = false, bool tex = false);
	
	int getTextWidth(const std::string& text, int fontSize);

	inline void setPathTex(const std::string& newPath) { pathTex = newPath; }

	void setFontSize(uint16_t size);

private:
	TTF_Font* font;
	std::string pathTex;
};
