#include "Menu.h"
#include "Globals.h"
#include "Button.h"
#include "MessageManager.h"

#include "ButtonMngr.h"
#include "TextureMngr.h"

Menu::Menu(SDL_Renderer* rend, std::shared_ptr<TextureMngr> texturemngr, std::shared_ptr<ButtonMngr> buttonmngr)
	: m_Renderer(rend), m_TextureMngr(texturemngr), m_ButtonMngr(buttonmngr)
{
	msgManager = std::make_unique<MessageManager>();

	if (!msgManager->setFont("../fonts/Roboto-Regular.ttf", 30)) {
		SDL_Log("Error");
		exit(1);
	}

	loadTextures();

	auto startButton = std::make_unique<Button>(550, 400, 200, 50, [this]() {startClicked = true; });
	startButton->loadTexture(m_Renderer, "../imgs/Menu/PlayBtn.png");
	auto exitButton = std::make_unique<Button>(550, 500, 200, 50, [this]() {exitClicked = true; });
	exitButton->loadTexture(m_Renderer, "../imgs/Menu/ExitBtn.png");

	m_ButtonMngr->addButton("startButton", std::move(startButton));
	m_ButtonMngr->addButton("exitButton", std::move(exitButton));

}

Menu::~Menu() {
	freeTextures();
	m_ButtonMngr->clean();
}

void Menu::loadTextures() {
	m_TextureMngr->loadTex("backGroundMenu", "../imgs/Menu/douglas_nunes.png");
}

void Menu::freeTextures() {
	m_TextureMngr->dropTex("backGroundMenu");
}

void Menu::Events(const SDL_Event& e) {
	m_ButtonMngr->updateAll(e);
}

void Menu::Render() {
	m_TextureMngr->draw("backGroundMenu", nullptr, nullptr);

	m_ButtonMngr->renderAll();

	msgManager->Render(m_Renderer, "Seminario Douglas", { 0, 230, 0 }, Global::DESIGN_WIDTH / 2 - 120, 50);
}

bool Menu::isStartClicked() const{
	return startClicked;
}

bool Menu::isExitClicked() const {
	return exitClicked;
}
