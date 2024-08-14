/*
	Feito by ME

	Admnistra o Computador:
		Carrega as texturas
		Cria os Bot�es do PC
		Encarrega-se da l�gica principal do PC


	Cara, isso ta dando muito trabalho sksks
*/


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

// Esses neg�cio grande ai
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

	// Atributos padr�o dos Print
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	SDL_Texture* Tex;
	int MaxY;
	bool Scroll;
	int ScrollMax;
};

/*

	� a classe Game 2.0, n�o faz sentido seu comportamento, mas funciona ent�o OK 


	� a parte mais legal do game
*/

class Computer
{
public:
	/*
	* 
	* Carrega o caminho das texturas do XML
	* 
	* Carrega a primeira textura (workspace1)
	* 
	* @param rend - SDL_Renderer, para ele conseguir renderizar coisa na tela
	* 
	* @return this
	*/
	Computer(SDL_Renderer* rend);

	// Fun��o Renderizadora principal
	void Render();

	// Inline Functions

	/*
	* 
	* Seta o Estado do computador
	* 
	* OBS: O Estado precisa ser V�LIDO, e precisa estar implementado
	* 
	* @param newState - Define um novo estado
	* 
	*/
	inline void setState(const std::string& newState) {
		m_ComputerState = newState;
		m_LoadNewScreen = true;
	};
	
	inline std::string getState() const {
		return m_ComputerState;
	}
	
	/*
	* Gerenciador principal de eventos do PC
	* 
	* Chama outros m�todos de events
	* 
	*/
	void Events(const SDL_Event& e);

private:
	// Essa classe usa muito o Renderer, ent�o ele se torna um atributo para facilitar
	SDL_Renderer* m_Rend;

	SDL_Rect printRect;
	SDL_Rect screenListaAfaz;

	std::string m_ComputerState;

	std::vector<PrintTexture> m_Textures;

	ButtonsMap m_ButtonsPtrMap;
	FilePrintMap m_PrintsMap;

	void ParseXML();

	// Renderizadores
	void RenderNotePad();
	void m_RenderTextures();
	void m_RenderMoldure();

	inline void DrawButtons() {
		SDL_SetRenderDrawColor(m_Rend, 255, 255, 255, 255);
		for (auto& b : m_ButtonsPtrMap) {
			b.second->Draw(m_Rend);
		}
	}

	inline void m_UpdateButtons(const SDL_Event& e) {
		for (auto& b : m_ButtonsPtrMap) {
			b.second->Update(e);
		}
	}
	
	// Fun��es de carregamento
	// Apenas deletam o que tinha e carregam o novo


	void m_CleanButtonMap();

	void m_LoadButtons();
	void m_LoadTexture();
	
	// Chama o LoadButtons e LoadTexture
	void LoadNewScreen();

	// Fun��es de evento

	// Verifica se o mouse est� dentro da area do pc, se n�o estiver ele n�o estar� vis�vel
	void m_MouseInComputer();
	
	/*
	* 
	* Gerencia o Scroll do Mouse
	* 
	* @param e - SDL_Event, � meio �bvio para que serve isso n�
	* 
	*/
	void m_MouseWhell(const SDL_Event& e);

	void m_UpdtDinamicBtsState();
	
	// Vari�veis de controle das janelas secund�rias da WORKSPACE2
	bool m_inLixeira{ false };
	bool m_inSecretPasta{ false };
	bool m_inListaAfazeres{ false };

	// Os bot�es estavam se deletando, ent�o tive que adicionar essa variavel de controle para poder
	// fazer eles serem deletados s� depois

	bool m_LoadNewScreen{ false };

	int m_ScrollControl{ 0 };

	// Ponteiros inteligentes

	std::unique_ptr<Notepad> m_Notepad;

	// � o que exibe as mensagens na tela
	std::unique_ptr<MessageManager> m_MsgManager;
};
