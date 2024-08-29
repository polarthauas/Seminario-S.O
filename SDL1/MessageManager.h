/*
	--------------
	Made by Thauan
	--------------


	Gerenciador de mensagens do game

	Utiliza um sistema de filas para gerenciar as mensagens,
	ap�s o usu�rio clicar enter ele apaga o primeiro elemento e vai para o pr�ximo (Propriedade b�sica das filas)


	Gerencia as fontes do jogo **INDEVIDAMENTE**

	

*/


#pragma once

#include "Globals.h"
#include "Message.h"

#include <queue>

class MessageManager
{
public:
	MessageManager() = default;
	~MessageManager();
	
	bool setFont(const std::string& fontPath, int fontSize);

	void render(SDL_Renderer* rend, const std::vector<std::string>& messages, const SDL_Color color, int x, int y);
	// Fun��o sobrecarregada para aceitar um vetor de string(v�rias linhas) ou apenas uma string(linha �nica)
	void render(SDL_Renderer* rend, const std::string& messages, const SDL_Color color, int x, int y);

	void renderAll(SDL_Renderer* rend);
	void updateAll();

	void popQueue();

	void processInput(const SDL_Event& e);

	void addMessage(std::unique_ptr<Text::Message> msg);

	void addDialogBox(std::unique_ptr<Text::Message> msg, const std::string& dialogBoxPath = "Assets/Game/dialog-box.png");

	void setFontSize(uint16_t size);

	inline int getFontSize() const { return m_FontSize; }

	inline TTF_Font* getFont() const { return m_Font; }


private:
	std::queue<std::unique_ptr<Text::Message>> messageQueue;

	int m_FontSize	;

	TTF_Font* m_Font { nullptr };
};
