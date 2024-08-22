#pragma once

#include "TextureMngr.h"
#include "Button.h"

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

class ButtonMngr {
public:
	ButtonMngr(SDL_Renderer* rend, std::shared_ptr<TextureMngr> textureMngr)
		: m_Rend(rend), m_TextureMngr(textureMngr) {}

	void addButton(const std::string& id, std::unique_ptr<Button> button);
	
	void dropButton(const std::string& id);
	void clean();

	void renderAll();

	void updateAll(const SDL_Event& e);

private:
	SDL_Renderer* m_Rend;
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::unordered_map<std::string, std::unique_ptr<Button>> m_Buttons;
};
