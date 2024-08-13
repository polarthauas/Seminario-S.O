#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>

#include "Button.h"
#include "MessageManager.h"

class MessageManager;

class Menu {
public:
	explicit Menu(SDL_Renderer* renderer);
	~Menu();

	void Events(const SDL_Event& e);
	void Render();

	bool isStartClicked() const;
	bool isExitClicked() const;

private:
	SDL_Renderer* renderer;
	SDL_Texture* backgroundTex;
	SDL_Texture* startButtonTex;
	SDL_Texture* exitButtonTex;

	SDL_Rect startButtonRect;
	SDL_Rect exitButtonRect;

	std::unique_ptr<Button> StartButton;
	std::unique_ptr<Button> ExitButton;

	std::unique_ptr<MessageManager> msgManager;
	
	bool startClicked;
	bool exitClicked;

	void loadTextures();
	void freeTextures();
};