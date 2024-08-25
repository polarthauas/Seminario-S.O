#include "Game.h"
#include "Globals.h"
#include "Douglas.h"
#include "Fases.h"
#include "mouse.h"
#include "Menu.h"
#include "Computer.h"

// Managers
#include "MessageManager.h"
#include "SoundMngr.h"
#include "ButtonMngr.h"
#include "QuestManager.h"
#include "TextAnimMngr.h"
#include "TextureMngr.h"

#include <iostream>
#include <cstdlib>	

Game::Game(){}

Game::~Game() {
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::Init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return false;
	}
	if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))) {
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

	SDL_GetWindowSize(m_Window, &Global::windowWidth, &Global::windowHeight);

	Global::douglasWidth = Global::resizeValue(Global::DESIGN_DOUGLAS_WIDTH, Global::RESIZE_MODE_WIDTH);
	Global::douglasHeight = Global::resizeValue(Global::DESIGN_DOUGLAS_WIDTH, Global::RESIZE_MODE_HEIGHT);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	m_MsgManager = std::make_shared<MessageManager>();
	
	m_MsgManager->setImageTexture(m_Renderer, "Assets/Douglas/7.png");
	
	m_TextureMngr = std::make_shared<TextureMngr>(m_Renderer);
	m_ButtonMngr = std::make_shared<ButtonMngr>(m_Renderer, m_TextureMngr);
	m_SoundMngr = std::make_shared<SoundMngr>();
	m_TextAnimMngr = std::make_shared<Text::TextAnimMngr>();

	if (!m_MsgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Erro ao carregar a fonte");
		return false;
	}

	m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr, m_SoundMngr);

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
	m_Menu.reset();
	m_Douglas.reset();
	m_Computer.reset();
	m_Fases.reset();

	m_TextAnimMngr.reset();

	m_ButtonMngr.reset();
	
	m_MsgManager.reset();
	m_SoundMngr.reset();
	m_TextureMngr.reset();

	if (m_Renderer) SDL_DestroyRenderer(m_Renderer);
	if (m_Window) SDL_DestroyWindow(m_Window);
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
				m_Menu.reset();
				
				setGameState(GameState::INGAME);
				m_Douglas = std::make_unique<Douglas>(m_Renderer, m_TextureMngr);
				m_Fases = std::make_unique<Fases>(m_Renderer);

				m_QuestMngr = std::make_shared<QuestManager>();
				
				m_TextAnimMngr->addTextAnimation("message1", { "Ola, eu sou o Douglas\n(Ignore minha calv�cie, j� vou fazer implante capilar)", {0, 0, 0}, 150, 500}, true, true);

				auto button = std::make_unique<Button>(595, 256, 85, 128, [this]() {
					m_TentouSair = true;
					});

				m_ButtonMngr->addButton("doorButton", std::move(button));
			}
			else if (m_Menu->isExitClicked()) {
				m_IsRunning = false;
			}
		}
		else if (m_GameState == GameState::INGAME) {
			m_Douglas->Event(e);

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_Fases.reset();
					m_Douglas.reset();

					auxControlGame = 0;

					m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr, m_SoundMngr);	

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
		m_ButtonMngr->updateAll(e);
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

		if (m_TentouSair) m_MsgManager->render(m_Renderer, "Tem nada pra fazer la fora n�o fi", { 255, 25, 56 }, 200, 300, true, true);

	}	
	else if (m_GameState == GameState::INCOMPUTER) {
		m_Computer->Render();

		controlGameMsgs(0);
	}

	m_ButtonMngr->renderAll();
	m_TextAnimMngr->renderAll(m_Renderer, m_MsgManager);
	if (m_QuestMenu) m_QuestMngr->Render(m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

void Game::Update()
{
	frameStart = SDL_GetTicks();

	m_TextAnimMngr->updateAll(m_SoundMngr);

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
	switch (auxControlGame) {
	
	case 0:
		if (cmd == 0) {
			m_Douglas->setState(DouglasState::LOOKING_FORWARD);
			m_Douglas->setCanControl(false);
		}
		else if (cmd == 1) {
			auxControlGame++;
			m_MsgManager->setFontSize(30);
			m_TextAnimMngr->clean();
			m_TextAnimMngr->addTextAnimation("message2", { "Vim ajudar na apresenta��o sobre...", {0, 0, 0}, 150, 500 }, true, true);
		}
 		
		break;
	case 1:
		if (cmd == 0) {
			
		}
		else if (cmd == 1) {
			auxControlGame++;
			m_TextAnimMngr->clean();
			m_TextAnimMngr->addTextAnimation("message3", { "Configura��es de Seguran�a e Privacidade no Windows", { 255, 0, 255 }, 150, 50 }, true, true);
		}
		break;
	case 2:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer, "Configura��es de Seguran�a e Privacidade no Windows", { 255, 0, 255 }, 150, 50);
		}
		else if (cmd == 1) {
			m_Douglas->jump();
			auxControlGame++;
			m_TextAnimMngr->clean();
			m_TextAnimMngr->addTextAnimation("message4", { "Vamos come�ar com a introdu��o sobre seguran�a\nPara isso vamos rodar o comando start ms-settings:windowsdefender", {205, 0, 0}, 100, 500 }, true, true);
		}
		break;
	
	case 3:
		if (cmd == 0) {
		} else if (cmd == 1) {
			runCmd("start ms-settings:windowsdefender");
			auxControlGame++;
			m_QuestMngr->AddQuest(QuestType::Main, "Entre no pc", "Clique no pc que ele abre");
			auto button = std::make_unique<Button>(85, 256, 85, 64, [this]() {
				auxControlGame++;
				m_QuestMngr->DropQuest("Entre no pc");
			});

			m_ButtonMngr->addButton("computerEnter", std::move(button));

			m_TextAnimMngr->clean();
			m_TextAnimMngr->addTextAnimation("message5", { "Agora que foi dada a introdu��o do tema, vamos nos aprofundar mais...", {20, 10, 56}, 100, 500 }, true, true);
			m_TextAnimMngr->addTextAnimation("message6", { "Nova miss�o adicionada! Pressione M para acessar o menu de miss�es", {255, 0, 0}, 100, 570 }, true, false);
		}
		break;

	case 4:
		if (cmd == 0) {
			/*m_MsgManager->render(
				m_Renderer, 
				"",
				{ 0, 10, 56 },
				100, 500, true, true
			);
			m_MsgManager->render(m_Renderer, "Nova miss�o adicionada! Pressione M para acessar o menu de miss�es", { 255, 0, 0 },
				100, 550, true, false);*/

		} else if (cmd == 1) {
			m_Douglas->setCanControl(true);
			auxControlGame++;
			m_TextAnimMngr->clean();
		}
		break;
	case 6:
		if (cmd == 0) {
			m_Fases.reset();
			 
			setGameState(GameState::INCOMPUTER);

			m_Computer = std::make_unique<Computer>(m_Renderer, m_TextureMngr, m_ButtonMngr);
			mouse = std::make_unique<Mouse>();

			m_ButtonMngr->clean();

			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

			auxControlGame++;
		}
		break;
	case 7:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer, "Aiai, novamente estou no pc, vamos navegar na internet", {0, 0, 0}, 160, 100,
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
			
			m_MsgManager->render(m_Renderer, "Vou pesquisar sobre o tratamento da calvice...", {0, 0, 0}, 250, 100,
				true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("INFIREFOXY2");
			auxControlGame++;
		}
		break;
	case 9:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer, "Agora basta dar enter para eu ter acesso ao tratamento SUPREMO contra a calvice", { 230, 50, 0 }, 250, 100,
				true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("BLUESCREEN");
			auxControlGame++;
		}

		break;
	case 10:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer, "Oh n�o, sabia que eu n�o deveria ter clicado no baner\n M�es solteiras que curam calvicie a 5km de voc� e desligado o windows defender",
				{ 255, 0, 255 }, 100, 50, true, true);
		} if (cmd == 1) {
			auxControlGame++;
		}
		break;
	case 11:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer, "Agora vou formatar o windows, ja volto...", {25, 50, 50},
				250, 50, true, true);
		}
		else if (cmd == 1) {
			m_Computer->setState("WORKSPACE2");
			auxControlGame++;
			m_MsgManager->setFontSize(25);
			//m_QuestMngr->AddQuest(QuestType::Main, "Entre nas configura��es!", "Clique no icone de configura��es para acessar o menu");
			//m_QuestMngr->AddQuest(QuestType::Secondary, "Explore os arquivos do Douglas", "Sai clicando em tudo ai");
		}
		break;
	case 12:
		if (cmd == 0) {
			m_MsgManager->render(m_Renderer,
				"Agora que EU formatei o Windows EU preciso de sua\najuda para configurar corretamente o WINDOWS DEFENDER e a manter minha privacidade,\nagora VOC� tem permiss�o para CONTROLAR o MEU pc\nM�ltiplas miss�es adicionadas!",
				{255, 0 , 0}, 250, 500, true, true);
		}
		else if (cmd == 1) auxControlGame++;

	case 13:

		break;
	default:
		break;
	}
}

