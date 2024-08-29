#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "Douglas.h"
#include "Fases.h"
#include "Menu.h"
#include "Computer.h"
#include "mouse.h"
#include "Game.h"

// Managers

class QuestManager;
class MessageManager;
class ButtonMngr;
class TextureMngr;
class SoundMngr;

namespace Text {
	class TextAnimMngr;
}

#include "mouse.h"

enum class GameState {
	INMENU,
	INGAME,
};

const SDL_Color SDL_WHITE = { 255, 255, 255 };
const SDL_Color SDL_RED = { 255, 0, 0 };
const SDL_Color SDL_BLUE = { 0, 0, 255 };
const SDL_Color SDL_BLACK = { 0, 0, 0 };
const SDL_Color SDL_GREEN = { 0, 255, 0 };
const SDL_Color SDL_PURPLE = { 255, 0, 255 };

class Game {
public:
	/**
	* 
	* Inicializa o Game
	* 
	* @param title - Titulo da janela
	* @param width - Largura da janela
	* @param hegith - Altura da janela
	* 
	* @return Se sucesso true se não false 
	* 
	*/
	bool Init(const char* title, int width, int height);

	bool initSDL();

	void quitSDL();
	
	/**
	*
	* Libera os recursos do game
	* 
	*/
	void cleanUp();

	inline bool getIsRunning() const { return m_IsRunning; }

	inline void exitGame() { m_IsRunning = false; }

	// Funções Principais

	void Event();
	void Render();
	void Update();

	void startGame(bool fisrt = false);

private:
	
	float deltaTime{ 0 };
	Uint32 lastFrameTime{ 0 };
	
	SDL_Window* m_Window{ nullptr };
	SDL_Renderer* m_Renderer{ nullptr };

	// Ponteiros inteligentes
	std::unique_ptr<Menu> m_Menu;
	std::unique_ptr<Fases> m_Fases;
	std::unique_ptr<Computer> m_Computer;
	std::unique_ptr<Douglas> m_Douglas;
	std::unique_ptr<Mouse> mouse;
	std::shared_ptr<QuestManager> m_QuestMngr;
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;
	std::shared_ptr<EventsMngr> m_EventsMngr;

	std::shared_ptr<MessageManager> m_MsgManager;

	void computerOn();
	void computerOff();

	GameState m_GameState{ GameState::INMENU };
	
	bool m_IsRunning{ false };
	bool m_QuestMenu{ false };

	bool m_ComputerOn{ false };
};
