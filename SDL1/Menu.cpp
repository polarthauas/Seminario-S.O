#include "Menu.h"
#include "Globals.h"
#include "Button.h"
#include "MessageManager.h"

Menu::Menu(SDL_Renderer* rend)
	: renderer(rend)
{

	msgManager = std::make_unique<MessageManager>();

	msgManager->setFont("../fonts/Roboto-Regular.ttf", 30);

	loadTextures();

	StartButton = std::make_unique<Button>(550, 400, 200, 50, [this]() {startClicked = true; }, startButtonTex);
	ExitButton = std::make_unique<Button>(550, 500, 200, 50, [this]() {exitClicked = true; }, exitButtonTex);
}

Menu::~Menu() {
	freeTextures();
}

void Menu::loadTextures() {
	backgroundTex = IMG_LoadTexture(renderer, "../imgs/Menu/douglas_nunes.jpg");

	exitButtonTex = IMG_LoadTexture(renderer, "../imgs/Menu/ExitBtn.png");

	startButtonTex = IMG_LoadTexture(renderer, "../imgs/Menu/PlayBtn.png");
}

void Menu::freeTextures() {
	SDL_DestroyTexture(backgroundTex);
	SDL_DestroyTexture(startButtonTex);
	SDL_DestroyTexture(exitButtonTex);

}

void Menu::Events(const SDL_Event& e) {
	StartButton->Update(e);
	ExitButton->Update(e);
}

void Menu::Render() {
	SDL_RenderCopy(renderer, backgroundTex, nullptr, nullptr);

	StartButton->Draw(renderer);
	ExitButton->Draw(renderer);

	msgManager->Render(renderer, "Seminario Douglas", { 0, 230, 0 }, windowWidth / 2 - 120, 50);
}

bool Menu::isStartClicked() const{
	return startClicked;
}

bool Menu::isExitClicked() const {
	return exitClicked;
}
