#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Message {
public:
	Message(const std::string& text, const SDL_Color color, int x, int y, bool square = false, bool hastexute = false)
		:b_Text(text), b_Color(color), b_X(x), b_Y(y), b_HasSquare(square), b_HasTexture(hastexute) {}

	virtual void update() = 0;

	virtual void draw(SDL_Renderer* rend, TTF_Font* font) = 0;

	void render(SDL_Renderer* rend, TTF_Font* font, std::string text = "");
	
	inline std::string getTex() const { return b_Text; }

protected:
	std::string b_Text;
	SDL_Color b_Color;	
	
	int b_X;
	int b_Y;
	int b_FontSize;

	bool b_HasSquare;
	bool b_HasTexture;
};


