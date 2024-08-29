#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>


class MessageManager;
class ButtonMngr;
class MessageManager;
class TextureMngr;
class EventsMngr;
class Game;

class Menu {
public:
	Menu(SDL_Renderer* renderer, std::shared_ptr<TextureMngr> texturemngr,
		std::shared_ptr<ButtonMngr> buttonmngr, Game* game);	

	void Render();

	void clean() {};

private:
	void start(std::shared_ptr<EventsMngr> m_EventsMngr, Game* game);

	SDL_Renderer* m_Renderer;

	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;
	std::shared_ptr<MessageManager> msgManager;
	
	bool startClicked;
	bool exitClicked;

	void loadTextures();
};