#pragma once

#include "MessageManager.h"

#include <vector>
#include <SDL2/SDL.h>
#include <string>
#include <memory>

class Notepad
{
public:
	
	const uint8_t CURSOR_WIDTH = 1;
	const uint8_t CURSOR_HEIGHT = 12;
	const uint8_t OFFY = 25;

	explicit Notepad(const SDL_Rect& r);
	~Notepad() = default;

	void render(SDL_Renderer* rend, const SDL_Rect& rCollision);
	void Events(const SDL_Event& e);

	void SetIsTyping(bool b);

	inline SDL_Rect getRect() const { return notepadRect; }

private:
	SDL_Rect notepadRect;
	SDL_Rect cursorRect;

	uint32_t linhaNote{ 0 };
	uint32_t maxLinha{ 15 };
	uint32_t cursorPos{ 0 };
	uint32_t cursorBlinkTime;

	int dragStartX{ 0 };
	int dragStartY{ 0 };
	int notePadStartX{ 0 };
	int notePadStartY{ 0 };


	bool cursorVisible = true;
	bool isDraggingNotePad = false;
	bool isTyping = false;

	std::vector<std::string> noteText;

	std::unique_ptr<MessageManager> msgManager;

	void updateCursor();
};

