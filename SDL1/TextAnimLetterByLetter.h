#pragma once

#include <SDL2/SDL.h>

#include "TextAnimation.h"

namespace Text {
	class LetterByLetter : public TextAnimation {
	public:
		LetterByLetter(const std::vector<std::string>& texts, const SDL_Color color, int x = 0, int y = 0, MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE)
			: TextAnimation(texts, color, x, y, 50, msgpostype) {

			startAnimation();
		}

		LetterByLetter(const std::string& text, const SDL_Color color, int x, int y,
			MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE)
			: TextAnimation(std::vector<std::string>{ text }, color, x, y, 50, msgpostype) {
			startAnimation();
		}

		~LetterByLetter() override {}

		void update() override;
		void draw(SDL_Renderer* rend, TTF_Font* font) override;

		void startAnimation() override;

		void onAnimationEnd() override {}

	private:
		void updateVisibleText();

		size_t m_CurrentLineIndex{ 0 };
		size_t m_CurrentTextIndex{ 0 };

		std::vector<std::string> m_VisibleText;
	};
}
