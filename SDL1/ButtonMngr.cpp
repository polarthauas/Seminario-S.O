#include "ButtonMngr.h"

#include "Computer.h"

void ButtonMngr::addButton(const std::string& id, std::unique_ptr<Button> button)
{
	if (m_Buttons.find(id) != m_Buttons.end()) {
		SDL_Log("ButtonMngr addButton: Botão: %s já existe! Impossível adicionar", id.c_str());
		return;
	}

	m_Buttons[id] = std::move(button);
}

void ButtonMngr::dropButton(const std::string& id)
{
	if (m_Buttons.find(id) == m_Buttons.end()) {
		SDL_Log("ButtonMngr dropButton: Botão: %s não existe! Impossível deletar", id.c_str());
		return;
	}

	m_Buttons.erase(id);
}

void ButtonMngr::clean()
{
	for (const auto& b : m_Buttons) {
		buttonsToDelete.push_back(b.first);
	}
}

void ButtonMngr::cleanNow() {
	m_Buttons.clear();
}

void ButtonMngr::renderAll()
{
	for (const auto& b : m_Buttons) {
		if (!b.second->draw(m_Rend)) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ButtonMngr renderAll: Botão: %s Erro: %s", b.first.c_str(), SDL_GetError());
		}
	}
}

void ButtonMngr::updateAll(const SDL_Event& e)
{
	for (const auto& b : m_Buttons) {
		b.second->update(e);
	}

	for (const auto& b : buttonsToDelete) {
		dropButton(b);
	}

	buttonsToDelete.clear();
}

void ButtonMngr::updatePosition(const std::string& id, int x, int y)
{
	if (m_Buttons.find(id) == m_Buttons.end()) {
		SDL_Log("ButtonMngr updatePosition: Botão: %s não encontrado", id.c_str());
		return;
	}

	m_Buttons[id]->setPosition(x, y);
}

void ButtonMngr::updateDinamicButtons(PrintTexture& p)
{
	for (auto& b : m_Buttons) {
		if (b.first.substr(0, 7) == "Lateral") continue;

		auto btnRect = b.second->GetRect();
		auto btnOriginRect = b.second->GetOriginRect();
		auto btnY = btnOriginRect.y - p.srcRect.y;

		if (btnY <= p.dstRect.y + p.dstRect.h && btnY >= p.dstRect.y) {
			int _height = 0;

			_height = std::min(p.dstRect.y + p.dstRect.h - btnY, btnOriginRect.h);

			b.second->SetClicable(true);
			b.second->SetVisible(true);

			// Ajusta a posição e a altura do botão
			b.second->setPosition(btnOriginRect.x, btnY, 0, _height);
		}
		else {
			b.second->SetVisible(false);
			b.second->SetClicable(false);
			b.second->resetPosition();
		}
	}
}

bool ButtonMngr::find(const std::string id)
{
	return (m_Buttons.find(id) != m_Buttons.end());
}
