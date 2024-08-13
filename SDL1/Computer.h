#pragma once

#include "Notepad.h"
#include "MessageManager.h"
#include "Button.h"

#include <SDL_image.h>
#include <SDL.h>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <SDL_mixer.h>

// Esses negócio grande ai
using ButtonsMap = std::map<std::string, Button*, std::less<>>;
using FilePrintMap = std::map<std::string, std::string, std::less<>>;

// Velocidade da rolagem das guias
const uint8_t SCROLL_SPEED = 15;

// Facilita o gerenciamento das Print
struct PrintTexture {
	PrintTexture(SDL_Texture* tex, SDL_Rect dstrect, SDL_Rect srcrect = { 0, 0, 0, 0 }, bool scroll = false, int maxy = 0)
		: srcRect(srcrect), dstRect(dstrect), Tex(tex), MaxY(maxy), Scroll(scroll)
	{
		ScrollMax = MaxY - srcRect.h;
	}

	SDL_Rect srcRect;
	SDL_Rect dstRect;
	SDL_Texture* Tex;
	int MaxY;
	bool Scroll;
	int ScrollMax;
};

/*

	É a classe Game 2.0, não faz sentido seu comportamento, mas funciona então OK 


	É a parte mais legal do game
*/

class Computer
{
public:
	Computer(SDL_Renderer* rend);

	void Render();
	
	void setState(const std::string& newState) {
		m_ComputerState = newState;
		LoadNew = true;
	};

	std::string getState() const { return m_ComputerState; };

	void Events(const SDL_Event& e);

private:
	SDL_Renderer* m_Rend;
	
	SDL_Rect printRect;
	SDL_Rect screenListaAfaz;

	std::string m_ComputerState;

	std::vector<PrintTexture> m_Textures;

	ButtonsMap m_ButtonsMap;
	FilePrintMap m_PrintsMap;

	void ParseXML();

	void RenderNotePad();
	void RenderTextures();
	void RenderMoldure();

	void LoadButtons();
	void LoadTexture();
	void LoadNewScreen();
	
	void MouseInComputer();
	void MouseWhell(const SDL_Event& e);

	bool inLixeira = false;
	bool inSecretPasta = false;
	bool inListaAfazeres = false;
	
	bool LoadNew = false;

	int m_ScrollControl{ 0 };

	std::unique_ptr<Notepad> m_Notepad;
	std::unique_ptr<MessageManager> msgManager;
};
