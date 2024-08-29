#include "FadeIn.h"

void Text::FadeIn::update()
{
	auto _currentTime = SDL_GetTicks();

	if (_currentTime - b_LastUpdateTime >= b_Delay) {
		b_LastUpdateTime = _currentTime;

		if (m_Alpha < 255) {
			m_Alpha += 5;
			if (m_Alpha > 255) {
				m_Alpha = 255;
			}
		}
		else {
			onAnimationEnd();
		}
	}
}

void Text::FadeIn::draw(SDL_Renderer* rend, TTF_Font* font)
{
	b_Color.a = m_Alpha;
	
	if(b_Color.a != 0)
	render(rend, font, b_Texts);
}

void Text::FadeIn::startAnimation()
{
	m_Alpha = 0;
	b_LastUpdateTime = SDL_GetTicks();
}

void Text::FadeIn::onAnimationEnd() {}
