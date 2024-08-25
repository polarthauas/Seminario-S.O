#pragma once

#include "Globals.h"

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class MessageManager
{
public:
	MessageManager();
	~MessageManager();
	
	bool setFont(const std::string& fontPath, int fontSize);

	void render(SDL_Renderer* rend, const std::string& message, SDL_Color color, int x, int y, bool square = false, bool tex = false);
	
	int getTextWidth(const std::string& text, int fontSize);

	void setImageTexture(SDL_Renderer* rend, const std::string& newPath);

	void setFontSize(uint16_t size);

private:
	SDL_Texture* _tex{ nullptr };
	TTF_Font* font{ nullptr };
};
