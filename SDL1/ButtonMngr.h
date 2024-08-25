#pragma once

#include "TextureMngr.h"
#include "Button.h"

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

struct PrintTexture;

class ButtonMngr {
public:
	ButtonMngr(SDL_Renderer* rend, std::shared_ptr<TextureMngr> textureMngr)
		: m_Rend(rend), m_TextureMngr(textureMngr) {}

	void addButton(const std::string& id, std::unique_ptr<Button> button);
	
	void dropButton(const std::string& id);
	void clean();

	void renderAll();

	void updateAll(const SDL_Event& e);

	void updatePosition(const std::string& id, int x, int y);

	void updateDinamicButtons(PrintTexture& p, int border_sizeX, int border_sizeY);

	bool find(const std::string id);

private:
	SDL_Renderer* m_Rend;
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::unordered_map<std::string, std::unique_ptr<Button>> m_Buttons;
};
