#include "Menu.h"
#include "Globals.h"
#include "MessageManager.h"

#include "ButtonMngr.h"
#include "TextureMngr.h"
#include "Game.h"

Menu::Menu(SDL_Renderer* rend, std::shared_ptr<TextureMngr> texturemngr, std::shared_ptr<ButtonMngr> buttonmngr, Game* game)
	: m_Renderer(rend), m_TextureMngr(texturemngr), m_ButtonMngr(buttonmngr)
{
	msgManager = std::make_shared<MessageManager>();

	if (!msgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Error");
		exit(1);
	}

	loadTextures();

	auto startButton = std::make_unique<Button>(550, 400, 200, 50, [this, game]() { game->startGame(true); });
	startButton->loadTexture(m_Renderer, "Assets/Menu/PlayBtn.png");
	auto exitButton = std::make_unique<Button>(550, 500, 200, 50, [this, game]() { game->exitGame(); });
	exitButton->loadTexture(m_Renderer, "Assets/Menu/ExitBtn.png");
	
	m_ButtonMngr->addButton("startButton", std::move(startButton));
	m_ButtonMngr->addButton("exitButton", std::move(exitButton));
}

void Menu::loadTextures() {
	m_TextureMngr->loadTex("backGroundMenu", "Assets/Menu/douglas_nunes.png");
}

void Menu::Render() {
	m_TextureMngr->draw("backGroundMenu", nullptr, nullptr);

	msgManager->render(m_Renderer, "Seminario Douglas", { 0, 230, 0 }, Global::DESIGN_WIDTH / 2 - 120, 50);
}

void Menu::start(std::shared_ptr<EventsMngr> m_EventsMngr, Game* game)
{

}
