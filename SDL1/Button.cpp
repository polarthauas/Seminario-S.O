#include "Button.h"
#include "Globals.h"

Button::Button(int x, int y, int width, int height, const std::function<void()>& onClick)
	: m_OnClick(onClick)
{
	// Ajusta conforme a resolução :)
	m_ButtonRect = { Global::resizeValue(x, Global::RESIZE_MODE_WIDTH), Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT), Global::resizeValue(width, Global::RESIZE_MODE_WIDTH), Global::resizeValue(height, Global::RESIZE_MODE_HEIGHT) };
	m_OriginRect = m_ButtonRect;
}

Button::~Button()
{
	if (m_Tex) SDL_DestroyTexture(m_Tex);
}

void Button::update(const SDL_Event& e)
{
	if (!m_Clicable) return;

	int mouseX;
	int mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	const bool isInside = Global::mouseInRect(mouseX, mouseY, m_ButtonRect);

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

bool Button::draw(SDL_Renderer* rend)
{
	if (!m_Visible) return true;

	if (m_Tex) {
		if (SDL_RenderCopy(rend, m_Tex, nullptr, &m_ButtonRect) != 0)
			return false;
	}
	else {
		if (SDL_RenderDrawRect(rend, &m_ButtonRect) != 0)
			return false;
	}

	return true;
}

