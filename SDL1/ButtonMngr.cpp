#include "ButtonMngr.h"

#include "Computer.h"

void ButtonMngr::addButton(const std::string& id, std::unique_ptr<Button> button)
{
	if (m_Buttons.find(id) != m_Buttons.end()) {
		SDL_Log("Id: %s já existe", id.c_str());
		return;
	}

	m_Buttons[id] = std::move(button);
}

void ButtonMngr::dropButton(const std::string& id)
{
	if (m_Buttons.find(id) == m_Buttons.end()) {
		SDL_Log("ID: %s não existe", id.c_str());
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

void ButtonMngr::updatePosition(const std::string& id, int x, int y)
{
	if (m_Buttons.find(id) == m_Buttons.end()) {
		SDL_Log("Id: %s não encontrado", id.c_str());
		return;
	}

	m_Buttons[id]->setPosition(x, y);
}

void ButtonMngr::updateDinamicButtons(PrintTexture& p, int border_sizeX, int border_sizeY)
{
	for (auto& b : m_Buttons) {
		if (b.first.substr(0, 7) == "Lateral") continue;

		auto btnY = b.second->GetOriginRect().y - p.srcRect.y;

		if (btnY <= p.dstRect.y + p.dstRect.h && btnY >= p.dstRect.y) {
			b.second->SetClicable(true);
			b.second->SetVisible(true);
			b.second->setPosition(b.second->GetOriginRect().x, btnY);
		}
		else {
			b.second->SetVisible(false);
			b.second->SetClicable(false);
			b.second->ResetPosition();
		}
	}
}

bool ButtonMngr::find(const std::string id)
{
	return (m_Buttons.find(id) != m_Buttons.end());
}
