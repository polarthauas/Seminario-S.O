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

#include "TextAnimLetterByLetter.h"


#include <iostream>
#include <cstdlib>	

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
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

	if (!m_MsgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Erro ao carregar a fonte");
		return false;
	}

	m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr);

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

	m_ButtonMngr.reset();
	m_MsgManager.reset();
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

		m_MsgManager->processInput(e);

		if (m_GameState == GameState::INMENU) {
			m_Menu->Events(e);
			if (m_Menu->isStartClicked()) {
				m_Menu.reset();
				
				setGameState(GameState::INGAME);
				m_Douglas = std::make_unique<Douglas>(m_Renderer, m_TextureMngr);
				m_Fases = std::make_unique<Fases>(m_Renderer);

				m_QuestMngr = std::make_shared<QuestManager>();
				
				auto button = std::make_unique<Button>(595, 256, 85, 128, [this]() {
					m_TentouSair = true;
					});

				m_ButtonMngr->addButton("doorButton", std::move(button));

				auto message1 = std::make_unique<Text::LetterByLetter>("Ola, eu sou o Douglas\n(Ignore minha calvície, já vou fazer implante capilar)", SDL_BLACK, 150, 500, true);

				auto message2 = std::make_unique<Text::LetterByLetter>( "Vim ajudar na apresentação sobre...", SDL_BLACK, 150, 500, true);

				auto message3 = std::make_unique<Text::LetterByLetter>("Configurações de Segurança e Privacidade no Windows", SDL_PURPLE, 150, 50, true);

				auto message4 = std::make_unique<Text::LetterByLetter>("Vamos começar com a introdução sobre segurança\nPara isso vamos rodar o comando start ms - settings:windowsdefender", SDL_RED, 100, 500, true);

				auto message5 = std::make_unique<Text::LetterByLetter>("Agora que foi dada a introdução do tema, vamos nos aprofundar mais...", SDL_BLACK, 100, 500, true);

				auto message6 = std::make_unique<Text::LetterByLetter>("Nova missão adicionadas! Pressione M para vê-la", SDL_GREEN, 100, 560, true);

				m_MsgManager->addMessage(std::move(message1));
				m_MsgManager->addMessage(std::move(message2));
				m_MsgManager->addMessage(std::move(message3));
				m_MsgManager->addMessage(std::move(message4));
				m_MsgManager->addMessage(std::move(message5));
				m_MsgManager->addMessage(std::move(message6));
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

					m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr);	

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

		if (m_TentouSair) m_MsgManager->render(m_Renderer, "Tem nada pra fazer la fora não fi", { 255, 25, 56 }, 200, 300);

	}	
	else if (m_GameState == GameState::INCOMPUTER) {
		m_Computer->Render();

		controlGameMsgs(0);
	}

	m_ButtonMngr->renderAll();
	m_MsgManager->renderAll(m_Renderer);

	if (m_QuestMenu) m_QuestMngr->Render(m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

void Game::Update()
{
	frameStart = SDL_GetTicks();

	m_MsgManager->updateAll();

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
		}
 		
		break;
	case 1:
		if (cmd == 0) {
			
		}
		else if (cmd == 1) {
			auxControlGame++;
		}
		break;
	case 2:
		if (cmd == 0) {
		}
		else if (cmd == 1) {
			m_Douglas->jump();
			auxControlGame++;
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
				m_MsgManager->popQueue();
				m_QuestMngr->DropQuest("Entre no pc");
			});

			m_ButtonMngr->addButton("computerEnter", std::move(button));
		}
		break;

	case 4:
		if (cmd == 1) {
			m_Douglas->setCanControl(true);
			auxControlGame++;
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

			auto message1 =  std::make_unique<Text::LetterByLetter>("Aiai, novamente estou no pc, vamos navegar na internet", SDL_BLACK, 160, 160, true);
			
			m_MsgManager->addMessage(std::move(message1));

			auxControlGame++;
		}
		break;
	case 7:
		if (cmd == 1) {
			auxControlGame++;;

			auto message1 = std::make_unique<Text::LetterByLetter>("Vou pesquisar sobre o tratamento da calvice...", SDL_BLACK, 250, 100, true);

			m_MsgManager->addMessage(std::move(message1));
		}
		break;
	case 8:
		if (cmd == 0) {
			if (m_Computer->getState() != "INFIREFOXY1")
				mouse->moveMouseSmoothly(700, 730, 1700);

			m_Computer->setState("INFIREFOXY1");
		}
		else if (cmd == 1) {
			m_Computer->setState("INFIREFOXY2");
			auxControlGame++;

			auto message1 = std::make_unique<Text::LetterByLetter>("Agora basta dar enter para eu ter acesso ao tratamento SUPREMO contra a calvice", SDL_RED, 200, 100, true);
			
			m_MsgManager->addMessage(std::move(message1));

		}
		break;
	case 9:
		if (cmd == 1) {
			m_Computer->setState("BLUESCREEN");
			auxControlGame++;

			auto message1 = std::make_unique<Text::LetterByLetter>("Oh não, sabia que eu não deveria ter clicado no baner\n Mães solteiras que curam calvicie a 5km de você e desligado o windows defender", SDL_PURPLE, 100, 50, true);

			m_MsgManager->addMessage(std::move(message1));
		}

		break;
	case 10:
		if (cmd == 0) {
		} if (cmd == 1) {
			auxControlGame++;

			auto message1 = std::make_unique<Text::LetterByLetter>("Agora vou formatar o Windows, ja volto...", SDL_BLACK, 250, 50, true);

			m_MsgManager->addMessage(std::move(message1));
		}
		break;
	case 11:
		if (cmd == 0) {
		}
		else if (cmd == 1) {
			m_Computer->setState("WORKSPACE2");
			auxControlGame++;
			m_MsgManager->setFontSize(25);
			m_QuestMngr->AddQuest(QuestType::Main, "Entre nas configuracões!", "Clique no icone de configurações para acessar o menu");
			m_QuestMngr->AddQuest(QuestType::Secondary, "Explore o PC", "Sai clicando em tudo ai");

			auto message1 = std::make_unique<Text::LetterByLetter>("Agora que EU formatei o Windows EU preciso de sua\najuda para configurar corretamente o WINDOWS DEFENDER e a manter minha privacidade,\nagora VOCÊ tem permissão para CONTROLAR o MEU pc\nMúltiplas missões adicionadas!", SDL_BLUE, 250, 500, true);

			m_MsgManager->addMessage(std::move(message1));
		}
		break;
	case 12:
		if (cmd == 0) {}
		
		else if (cmd == 1) {
			auxControlGame++;
			m_MsgManager->popQueue();
		}
	default:
		break;
	}
}

