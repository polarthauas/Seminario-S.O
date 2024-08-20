#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>

#include "MessageManager.h"
#include "ButtonMngr.h"
#include "TextureMngr.h"

class MessageManager;
class TextureMngr;

class Menu {
public:
	explicit Menu(SDL_Renderer* renderer, std::shared_ptr<TextureMngr> texturemngr, std::shared_ptr<ButtonMngr>);
	~Menu();

	void Events(const SDL_Event& e);
	void Render();

	bool isStartClicked() const;
	bool isExitClicked() const;

private:
	SDL_Renderer* m_Renderer;
	
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;

	SDL_Texture* backgroundTex;
	SDL_Texture* startButtonTex;
	SDL_Texture* exitButtonTex;	

	SDL_Rect startButtonRect;
	SDL_Rect exitButtonRect;

	std::unique_ptr<MessageManager> msgManager;
	
	bool startClicked;
	bool exitClicked;

	void loadTextures();
	void freeTextures();
};