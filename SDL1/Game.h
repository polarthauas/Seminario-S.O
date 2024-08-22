#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>

#include "Menu.h"
#include "Computer.h"
#include "Fases.h"

// Managers

#include "ButtonMngr.h"
#include "TextureMngr.h"
#include "MessageManager.h"
#include "QuestManager.h"


#include "mouse.h"

enum class GameState {
	INMENU,
	INGAME,
	INCOMPUTER
};

class Game {
public:
	Game();
	~Game();

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
	
	/**
	*
	* Libera os recursos do game
	* 
	*/
	void cleanUp();

	inline bool GetIsRunning() const { return m_IsRunning; }

	inline GameState GetGameState() const { return m_GameState; };
	
	// Funções Principais

	void Event();
	void Render();
	void Update();

private:
	
	// Provavelmente precisa de rework
	void controlGameMsgs(int cmd);

	void runCmd(const std::string& cmd) const;

	void setGameState(GameState NewState);

	Uint32 frameStart{ 0 };
	int frameTime{ 0 };

	uint32_t LastFrameTime{ SDL_GetTicks() };

	// Sla pq ta aqui, só tem 1 fase
	int16_t levelNum{ 0 };
	
	SDL_Window* m_Window{ nullptr };
	SDL_Renderer* m_Renderer{ nullptr };

	// Ponteiros inteligentes para o fases e menu
	std::unique_ptr<Menu> m_Menu;
	std::unique_ptr<Fases> m_Fases;
	std::unique_ptr<Computer> m_Computer;
	std::unique_ptr<Douglas> m_Douglas;
	std::unique_ptr<Mouse> mouse;
	
	std::shared_ptr<QuestManager> m_QuestMngr;
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;

	std::unique_ptr<MessageManager> m_MsgManager;

	GameState m_GameState{ GameState::INMENU };

	std::vector<Button> m_Buttons;	
	
	int auxControlGame{ 0 };
	
	bool m_IsRunning{ false };
	bool m_QuestMenu{ false };
	bool m_TentouSair{ false };
};
