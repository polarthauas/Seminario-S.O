/*
	Feito by ME

	Admnistra o Computador:
		Carrega as texturas
		Cria os Bot�es do PC
		Encarrega-se da l�gica principal do PC


	Cara, isso ta dando muito trabalho sksks
*/


#pragma once

#include "Globals.h"
#include "Notepad.h"

#include <unordered_map>

class TextureMngr;
class Button;
class ButtonMngr;
class MessageManager;

// Esses neg�cio grande ai
using FilePrintMap = std::unordered_map<std::string, std::string>;

// Velocidade da rolagem das guias
static const uint8_t SCROLL_SPEED = 15;

static const int BORDER_SIZE = 20;
static const int WINDOWSBAR_SIZE = 43;

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
	Computer(SDL_Renderer* rend, std::shared_ptr<TextureMngr> textureMngr, std::shared_ptr<ButtonMngr> buttonmngr, std::shared_ptr<MessageManager> msgMngr);

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

	FilePrintMap m_PrintsMap;

	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;

	/*
	* 
	* 0 - SETTINGS
	* 1 - WINDOWSDEFENDER
	* 2 - WINDOWSDEFENDERVIRUS
	* 
	*/
	std::vector<std::string> m_LocaisJaVisitados;

	bool findLocal(const std::string& local) {
		for (const auto& l : m_LocaisJaVisitados) {
			if (l == local) return true;
		}

		return false;
	}

	void ParseXML();

	// Renderizadores
	void RenderNotePad();
	void _RenderTextures();
	void _RenderMoldure();
	void _RenderSecretFolder();

	// Fun��es de carregamento
	// Apenas deletam o que tinha e carregam o novo

	void m_CleanButtonMap();

	void m_LoadButtons();
	void m_LoadTexture();

	void _LoadDefenderLateral();
	
	void _LoadExitBtn();
	void _LoadDefenderLateralBtns();

	inline void _LoadWindowsBar() {
		// Cria a barra do windows

		SDL_Rect bar = Global::resizeRect({ BORDER_SIZE, Global::windowHeight - BORDER_SIZE - WINDOWSBAR_SIZE, Global::windowWidth - 2 * BORDER_SIZE, WINDOWSBAR_SIZE});

		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_BAR"].c_str()), bar);
	
	}

	// Chama o LoadButtons e LoadTexture
	void LoadNewScreen();

	// Fun��es de evento

	// Verifica se o mouse est� dentro da area do pc, se n�o estiver ele n�o estar� vis�vel
	void MouseInComputer();
	
	/*
	* 
	* Gerencia o Scroll do Mouse
	* 
	* @param e - SDL_Event, � meio �bvio para que serve isso n�
	* 
	*/
	void MouseWhell(const SDL_Event& e);

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
	std::shared_ptr<MessageManager> m_MsgManager;
};
