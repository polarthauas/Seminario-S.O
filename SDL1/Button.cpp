#include "Button.h"
#include "Globals.h"
#include <string>

Button::Button(int x, int y, int width, int height, const std::function<void()>& onClick)
	: m_OnClick(onClick)
{
	// Ajusta conforme a resolução :)
	m_ButtonRect = { Global::resizeValue(x, Global::RESIZE_MODE_WIDTH), Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT), Global::resizeValue(width, Global::RESIZE_MODE_WIDTH), Global::resizeValue(height, Global::RESIZE_MODE_HEIGHT) };
	m_OriginRect = m_ButtonRect;
}

void Button::Update(const SDL_Event& e)
{
	int mouseX;
	int mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	const bool isInside = Global::clickedRect(mouseX, mouseY, m_ButtonRect);

	if (!isInside) {
		m_IsClicked = false;
		return;
	}

	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
		m_IsClicked = true;
	}

	else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && m_IsClicked) {
		m_IsClicked = false;
		m_OnClick();
	}
}

void Button::Draw(SDL_Renderer* rend)
{
	if (!m_Visible) return;

	if (m_Tex) {
		SDL_RenderCopy(rend, m_Tex, nullptr, &m_ButtonRect);
	}
	else {
		SDL_RenderDrawRect(rend, &m_ButtonRect);
	}
}

// Fundamental para os botões dinâmicos
// Reseta a posição do botão para o original
void Button::ResetPosition() {
	m_ButtonRect = m_OriginRect;
}
