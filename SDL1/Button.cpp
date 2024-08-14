#include "Button.h"
#include "Globals.h"
#include <string>

Button::Button(int x, int y, int width, int height, const std::function<void()>& onClick, SDL_Texture* tex)
	: m_Tex(tex), m_OnClick(onClick)
{
	// Ajusta conforme a resolução :)
	m_ButtonRect = { calcAlterWindowSize(x, 'w'), calcAlterWindowSize(y, 'h'), calcAlterWindowSize(width, 'w'), calcAlterWindowSize(height, 'h') };
	m_OriginRect = m_ButtonRect;
}

void Button::Update(const SDL_Event& e)
{
	int mouseX;
	int mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	const bool isInside = mouseX > m_ButtonRect.x && mouseY > m_ButtonRect.y && mouseX < m_ButtonRect.x + m_ButtonRect.w && mouseY < m_ButtonRect.y + m_ButtonRect.h;

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
