#include "Game.h"
#include "Globals.h"
#include "mouse.h"
#include "Menu.h"

// Managers
#include "MessageManager.h"
#include "SoundMngr.h"
#include "ButtonMngr.h"
#include "QuestManager.h"
#include "TextureMngr.h"
#include "DialogBox.h"
#include "TextAnimLetterByLetter.h"
#include "FadeIn.h"

bool Game::Init(const char* title, int width, int height) {
	initSDL();

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

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	m_MsgManager = std::make_shared<MessageManager>();
	m_TextureMngr = std::make_shared<TextureMngr>(m_Renderer);
	m_ButtonMngr = std::make_shared<ButtonMngr>(m_Renderer, m_TextureMngr);

	if (!m_MsgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Erro ao carregar a fonte");
		return false;
	}

	m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr, this);

	return m_IsRunning = true;
}

bool Game::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		return false;
	}
	if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG)) {
		std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		return false;
	}
	if (TTF_Init() != 0) {
		SDL_Log(TTF_GetError());
		return false;
	}

	return true;
}

void Game::quitSDL()
{
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::startGame(bool fisrt)
{
	m_Menu.reset();

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	m_GameState = GameState::INGAME;
	m_Douglas = std::make_unique<Douglas>(m_TextureMngr);
	m_Fases = std::make_unique<Fases>(m_Renderer);

	m_QuestMngr = std::make_shared<QuestManager>();

	m_MsgManager->setFontSize(28);

	m_ButtonMngr->clean();

	auto button = std::make_unique<Button>(595, 256, 85, 128, [this]() {
		auto message1 = std::make_unique<Text::FadeIn>(std::vector<std::string>{ "Tem nada para fazer lá fora não" }, SDL_RED, 150, 500);

		m_MsgManager->addDialogBox(std::move(message1));
		});

	m_ButtonMngr->addButton("doorButton", std::move(button));
	
	if (!fisrt) {
		auto button1 = std::make_unique<Button>(85, 256, 85, 64, [this]() {
			m_MsgManager->popQueue();
			m_QuestMngr->DropQuest("Entre no pc");
			computerOn();
			});

		m_ButtonMngr->addButton("computerEnter", std::move(button1));

		return;
	}

	auto message1 = std::make_unique<Text::LetterByLetter>(
		std::vector<std::string>{"Olá, eu sou o Douglas", "Ignore minha calvície, já vou fazer implante capilar" },
		SDL_BLACK, -1, -1);

	auto message2 = std::make_unique<Text::FadeIn>("Vim ajudar na apresentação sobre...", SDL_BLACK, 150, 500, Text::MESSAGE_REPOSITION_DIALOGBOX);

	auto message3 = std::make_unique<Text::LetterByLetter>("Configurações de Segurança e Privacidade no Windows",
		SDL_PURPLE, 150, 50, Text::MESSAGE_REPOSITION_DIALOGBOX);

	auto message4 = std::make_unique<Text::LetterByLetter>("Vou mexer no pc agr",
		SDL_WHITE, 150, 500, Text::MESSAGE_REPOSITION_DIALOGBOX);

	auto message6 = std::make_unique<Text::LetterByLetter>("Nova missão adicionadas! Pressione M para vê-la",
		SDL_GREEN, 100, 500, Text::MESSAGE_REPOSITION_DIALOGBOX);

	message6->setOnDestroyCallback([this](Text::Message* msg) {
		m_MsgManager->setFontSize(25);
		m_QuestMngr->AddQuest(QuestType::Main, "Entre nas configuracões!", "Clique no icone de configurações para acessar o menu");
		m_QuestMngr->AddQuest(QuestType::Secondary, "Explore o PC", "Sai clicando em tudo ai");

		auto button = std::make_unique<Button>(85, 256, 85, 64, [this]() {
			m_MsgManager->popQueue();
			m_QuestMngr->DropQuest("Entre no pc");
			computerOn();
			});

		m_ButtonMngr->addButton("computerEnter", std::move(button));

		m_Douglas->setCanControl(true);

		});

	
	m_MsgManager->addDialogBox(std::move(message1));
	m_MsgManager->addDialogBox(std::move(message2));
	m_MsgManager->addDialogBox(std::move(message3));
	m_MsgManager->addDialogBox(std::move(message6));
}

void Game::computerOn()
{
	m_ComputerOn = true;

	m_Fases.reset();
	m_Douglas.reset();

	m_Computer = std::make_unique<Computer>(m_Renderer, m_TextureMngr, m_ButtonMngr, m_MsgManager);
	mouse = std::make_unique<Mouse>();

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	auto message1 = std::make_unique<Text::LetterByLetter>("Aiai, novamente estou no pc, vamos navegar na internet", SDL_BLACK, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX);

	message1->setOnDestroyCallback([this](Text::Message* msg) {
		if (m_Computer->getState() != "INFIREFOXY1")
			mouse->moveMouseSmoothly(700, 730, 1700);

		m_Computer->setState("INFIREFOXY1");

		});

	auto message2 = std::make_unique<Text::LetterByLetter>("Vou pesquisar sobre o tratamento da calvice...", SDL_BLACK, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX);

	message2->setOnDestroyCallback([this](Text::Message* msg) {
		m_Computer->setState("INFIREFOXY2");
		});

	auto message3 = std::make_unique<Text::LetterByLetter>("Agora basta dar enter para eu ter acesso ao tratamento SUPREMO contra a calvice",
		SDL_RED, 200, 100, Text::MESSAGE_REPOSITION_NONE);

	message3->setOnDestroyCallback([this](Text::Message* msg) {
		m_Computer->setState("BLUESCREEN");
		});

	auto message4 = std::make_unique<Text::LetterByLetter>(
		std::vector<std::string>{"Oh não, sabia que eu não deveria ter clicado no banner"
		, "Mães solteiras que curam calvicie a 5km de você e desligado o windows defender"}, SDL_PURPLE, 100, 50, Text::MESSAGE_REPOSITION_DIALOGBOX);

	auto message5 = std::make_unique<Text::LetterByLetter>("Agora vou formatar o Windows, ja volto...", SDL_BLACK, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX);

	auto message6 = std::make_unique<Text::LetterByLetter>(
		std::vector<std::string>{"Agora que EU formatei o Windows EU preciso de sua", "ajuda para configurar corretamente o WINDOWS DEFENDER e a manter minha privacidade", "agora VOCÊ tem permissão para CONTROLAR o MEU pc", "Múltiplas missões adicionadas!"}, SDL_BLUE, -1, -1, Text::MESSAGE_REPOSITION_DIALOGBOX);

	message5->setOnDestroyCallback([this](Text::Message* msg) {
		m_Computer->setState("WORKSPACE2");
		});

	m_MsgManager->addDialogBox(std::move(message1));
	m_MsgManager->addDialogBox(std::move(message2));
	m_MsgManager->addDialogBox(std::move(message3));
	m_MsgManager->addDialogBox(std::move(message4));
	m_MsgManager->addDialogBox(std::move(message5));
	m_MsgManager->addDialogBox(std::move(message6));
}

void Game::computerOff()
{
	m_Computer.reset();
	
	m_ComputerOn = false;

	m_ButtonMngr->clean();
	m_TextureMngr->clean();

}

void Game::cleanUp() {
	m_Douglas.reset();
	m_Computer.reset();
	m_Fases.reset();

	m_Menu.reset();
	m_ButtonMngr.reset();
	m_MsgManager.reset();
	m_TextureMngr.reset();

	if (m_Renderer) SDL_DestroyRenderer(m_Renderer);
	if (m_Window) SDL_DestroyWindow(m_Window);

	quitSDL();
}

void Game::Event() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			m_IsRunning = false;
		}

		m_MsgManager->processInput(e);
		m_ButtonMngr->updateAll(e);

		if (m_GameState == GameState::INGAME) {
			if (m_ComputerOn) {
				m_Computer->Events(e);

				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						computerOff();
						startGame();
						break;
					}
				}

			}else {
				m_Douglas->Event(e);

				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						m_Fases.reset();
						m_Douglas.reset();
						m_Menu = std::make_unique<Menu>(m_Renderer, m_TextureMngr, m_ButtonMngr, this);

						m_GameState = GameState::INMENU;
						break;
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
}

void Game::Update()
{
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastFrameTime) / 1000.0f; 
	lastFrameTime = currentTime;

	m_MsgManager->updateAll();

	if (m_GameState == GameState::INGAME) {
		if (!m_ComputerOn) {
			m_Douglas->Update(deltaTime);
		}
	}
}

// Renderes

void Game::Render() {
	SDL_RenderClear(m_Renderer);

	if (m_GameState == GameState::INMENU) {
		m_Menu->Render();
	}
	else if (m_GameState == GameState::INGAME) {
		if (m_ComputerOn) {
			m_Computer->Render();
		}
		else {
			m_Fases->draw(m_Renderer, m_Douglas.get());
			m_Douglas->Render();
		}	
	}	

	m_ButtonMngr->renderAll();
	m_MsgManager->renderAll(m_Renderer);

	if (m_QuestMenu) m_QuestMngr->Render(m_Renderer);

	SDL_RenderPresent(m_Renderer);
}
