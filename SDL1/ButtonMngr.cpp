#include "ButtonMngr.h"

void ButtonMngr::addButton(const std::string& id, std::unique_ptr<Button> button)
{
	if (m_Buttons.find(id) != m_Buttons.end()) {
		SDL_Log("Esse id já existe");
		return;
	}

	m_Buttons[id] = std::move(button);
}

void ButtonMngr::dropButton(const std::string& id)
{
	if (m_Buttons.find(id) == m_Buttons.end()) {
		SDL_Log("Este Id não existe");
		return;
	}

	m_Buttons.erase(id);
}

void ButtonMngr::clean()
{
	m_Buttons.clear();
}

void ButtonMngr::renderAll()
{
	for (const auto& b : m_Buttons) {
		b.second->Draw(m_Rend);
	}
}

void ButtonMngr::updateAll(const SDL_Event& e)
{
	for (const auto& b : m_Buttons) {
		b.second->Update(e);
	}
}
