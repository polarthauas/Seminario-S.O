#include "Game.h"
#include "Globals.h"
#include "Douglas.h"
#include "Fases.h"
#include "MessageManager.h"
#include "mouse.h"
#include "Menu.h"
#include "Computer.h"

#include "Button.h"

#include <iostream>
#include <cstdlib>	

Game::Game(){}

Game::~Game() {
	cleanUp();
}

bool Game::Init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
		return false;
	}

	if (TTF_Init() != 0) {
		SDL_Log(TTF_GetError());
		return false;
	}

	m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

	if (m_Window == nullptr) {
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	if (m_Renderer == nullptr) {
		return false;
	}

	if (SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) {
		SDL_Log("Erro ao colocar em fullscreen");
		return false;
	}

	SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	m_MsgManager = new MessageManager();
	
	if (m_MsgManager == nullptr) {
		SDL_Log("Erro ao criar o MessageManager");
		return false;
	}

	if (!m_MsgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Erro ao carregar a fonte");
		return false;
	}

	m_Menu = std::make_unique<Menu>(m_Renderer);

	return m_IsRunning = true;
}

void Game::setGameState(GameState NewState) {
	m_GameState = NewState;
}

void Game::runCmd(const std::string& cmd) const
{
	std::system(cmd.c_str());
}

void Game::cleanUp() {

	if(m_MsgManager) delete m_MsgManager;

	if (m_Renderer) SDL_DestroyRenderer(m_Renderer);
	if (m_Window) SDL_DestroyWindow(m_Window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::Event() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			m_IsRunning = false;
		}

		if (m_GameState == GameState::INMENU) {
			m_Menu->Events(e);
			if (m_Menu->isStartClicked()) {
				setGameState(GameState::INGAME);
				m_Douglas = std::make_unique<Douglas>(m_Renderer);
				m_Fases = std::make_unique<Fases>(m_Renderer);
				m_QuestMngr = std::make_unique<QuestManager>();

				m_Buttons.emplace_back(595, 256, 85, 128, [this]() {
					m_TentouSair = true;
				});
				
				m_Menu.reset();
				
			}
			else if (m_Menu->isExitClicked()) {
				m_IsRunning = false;
			}
		}
		else if (m_GameState == GameState::INGAME) {
			m_Douglas->Event(e);
			for (auto& p : m_Buttons) p.Update(e);

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_Fases.reset();
					m_Douglas.reset();

					auxControlGame = 0;

					m_Menu = std::make_unique<Menu>(m_Renderer);	

					setGameState(GameState::INMENU);
					break;
				case SDLK_RETURN:
					m_TentouSair = false;

					controlGameMsgs(1);
					break;
				case SDLK_DELETE:
					auxControlGame--;

					break;
				case SDLK_m:
					m_QuestMenu = !m_QuestMenu;

				default:
					break;
				}
			}
		}
		else if (m_GameState == GameState::INCOMPUTER) {
			m_Computer->Events(e);

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:
					if (m_TentouSair) m_TentouSair = false;

					controlGameMsgs(1);
					break;
				case SDLK_m:
					m_QuestMenu = !m_QuestMenu;

				default:
					break;
				}
			}
		}
		if (m_QuestMenu) m_QuestMngr->Event(e);
	}
}

void Game::Render() {
	SDL_RenderClear(m_Renderer);

	if (m_GameState == GameState::INMENU) {
		m_Menu->Render();
	}
	else if (m_GameState == GameState::INGAME) {
		m_Fases->Render(m_Renderer, m_Douglas.get()); 
		m_Douglas->Render();

		controlGameMsgs(0);

		for (auto& p : m_Buttons) p.Draw(m_Renderer);
		if (m_TentouSair) m_MsgManager->Render(m_Renderer, "Tem nada pra fazer la fora não fi", { 255, 25, 56 }, 200, 300, true, true);

	}	
	else if (m_GameState == GameState::INCOMPUTER) {
		m_Computer->Render();

		controlGameMsgs(0);
	}

	if (m_QuestMenu) m_QuestMngr->Render(m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

void Game::Update()
{
	frameStart = SDL_GetTicks();

	if (m_GameState == GameState::INGAME) {
		m_Douglas->Update();
	}
		
	frameTime = SDL_GetTicks() - frameStart;

	if (1000 / 60 > frameTime) {
		SDL_Delay(1000 / 60 - frameTime);
	}
}

void Game::controlGameMsgs(int cmd)
{
	m_MsgManager->setPathTex("../imgs/Douglas/7.png");
	switch (auxControlGame) {
	
	case 0:
		if (cmd == 0) {
			m_Douglas->setState(DouglasState::LOOKING_FORWARD);
			m_Douglas->setCanControl(false);
			m_MsgManager->Render(m_Renderer, "Eae, tudo bem? Eu sou o Douglas \n(Ignore minha calvície, já vou fazer implante capilar)\nPressione enter para  continuar",
				{ 0, 0, 0 }, calcAlterWindowSize(150, 'w'), calcAlterWindowSize(500, 'h') , true, true);
		}
		else if (cmd == 1) { auxControlGame++; m_MsgManager->setFontSize(30); }
 		
		break;
	case 1:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Vim ajudar na apresentação do trabalho sobre...",
				{ 0, 0, 0 }, calcAlterWindowSize(150, 'w'),
				calcAlterWindowSize(500, 'h') , true, true);

		}
		else if (cmd == 1) auxControlGame++; 
		break;
	case 2:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Configurações de Segurança e Privacidade no Windows", { 255, 0, 255 }, 150, 50);
			
		}
		else if (cmd == 1) {
			m_Douglas->jump();
			auxControlGame++;
		}
		break;
	
	case 3:
		if (cmd == 0) {
			m_MsgManager->Render(
				m_Renderer, 
				"Vamos começar com a introdução sobre segurança\nPara isso vamos rodar o comando start ms-settings:windowsdefender",
				{ 205, 0, 0 },
				calcAlterWindowSize(100, 'w'), calcAlterWindowSize(500, 'h'), true, true
			);

		} else if (cmd == 1) {
			runCmd("start ms-settings:windowsdefender");
			auxControlGame++;
			m_QuestMngr->AddQuest(QuestType::Main, "Entre no pc", "Clique no pc que ele abre");
			m_Buttons.emplace_back(85, 256, 85, 64, [this]() {
				auxControlGame++;
				m_QuestMngr->DropQuest("Entre no pc");
			});
		}
		break;

	case 4:
		if (cmd == 0) {
			m_MsgManager->Render(
				m_Renderer, 
				"Agora que foi dada a introdução do tema, vamos nos aprofundar mais...",
				{ 0, 10, 56 },
				calcAlterWindowSize(100, 'w'), calcAlterWindowSize(500, 'h'), true, true
			);
			m_MsgManager->Render(m_Renderer, "Nova missão adicionada! Pressione M para acessar o menu de missões", { 255, 0, 0 },
				calcAlterWindowSize(100, 'w'), calcAlterWindowSize(550, 'h'), true, false);

		} else if (cmd == 1) {
			m_Douglas->setCanControl(true);
			auxControlGame++;
		}
		break;
	case 6:
		if (cmd == 0) {
			m_Fases.reset();
			 
			setGameState(GameState::INCOMPUTER);

			m_Computer = std::make_unique<Computer>(m_Renderer);
			mouse = std::make_unique<Mouse>(m_Renderer);

			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

			auxControlGame++;
		}
		break;
	case 7:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Aiai, novamente estou no pc, vamos navegar na internet", {0, 0, 0}, 160, 100,
				true, true);
		}
		else if (cmd == 1) {
			auxControlGame++;;
		}
		break;
	case 8:
		if (cmd == 0) {
			if (m_Computer->getState() != "INFIREFOXY1")
				mouse->moveMouseSmoothly(700, 730, 1700);

			m_Computer->setState("INFIREFOXY1");
			
			m_MsgManager->Render(m_Renderer, "Vou pesquisar sobre o tratamento da calvice...", {0, 0, 0}, 250, 100,
				true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("INFIREFOXY2");
			auxControlGame++;
		}
		break;
	case 9:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Agora basta dar enter para eu ter acesso ao tratamento SUPREMO contra a calvice", { 230, 50, 0 }, 250, 100,
				true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("BLUESCREEN");
			auxControlGame++;
		}

		break;
	case 10:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Oh não, sabia que eu não deveria ter clicado no baner\n Mães solteiras que curam calvicie a 5km de você e desligado o windows defender",
				{ 255, 0, 255 }, 100, 50, true, true);
		} if (cmd == 1) {
			auxControlGame++;
		}
		break;
	case 11:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer, "Agora vou formatar o windows, ja volto...", {25, 50, 50},
				250, 50, true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("WORKSPACE2");
			auxControlGame++;
			m_MsgManager->setFontSize(25);
			//m_QuestMngr->AddQuest(QuestType::Main, "Entre nas configurações!", "Clique no icone de configurações para acessar o menu");
			//m_QuestMngr->AddQuest(QuestType::Secondary, "Explore os arquivos do Douglas", "Sai clicando em tudo ai");
		}
		break;
	case 12:
		if (cmd == 0) {
			m_MsgManager->Render(m_Renderer,
				"Agora que EU formatei o Windows EU preciso de sua\najuda para configurar corretamente o WINDOWS DEFENDER e a manter minha privacidade,\nagora VOCÊ tem permissão para CONTROLAR o MEU pc\nMúltiplas missões adicionadas!",
				{255, 0 , 0}, 250, 500, true, true);
		}
		else if (cmd == 1) auxControlGame++;

	case 13:

		break;
	default:
		break;
	}
}

