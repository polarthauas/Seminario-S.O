#pragma once

#include <SDL2/SDL.h>

#include "TextAnimation.h"

namespace Text {
	class LetterByLetter : public TextAnimation{
	public:
		LetterByLetter(const std::string& text, const SDL_Color color, int x, int y, bool square = false, bool hasTexture = false) 
			: TextAnimation(text, color, x, y, 50, square, hasTexture) {
			
			startAnimation();
		}

		~LetterByLetter() override {}

		void update() override;
		void draw(SDL_Renderer* rend, TTF_Font* font) override;

		void startAnimation() override;

		void resetAnimation() override {}

		void onAnimationEnd() override {}
		
	private:
		size_t m_CurrentIndex{ 0 };
	};
}
