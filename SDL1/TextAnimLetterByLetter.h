#pragma once

#include "TextAnimation.h"

namespace Text {
	class LetterByLetter : public Text::TextAnimation {
	public:
		LetterByLetter(const std::string& text, SDL_Color color, int x, int y, bool square = false, bool hasTexture = false) 
			: TextAnimation(text, color, x, y, 50, square, hasTexture) {}

		void startAnimation() override;

		void update(std::shared_ptr<SoundMngr> soundMngr) override;

		void render(SDL_Renderer* rend, std::shared_ptr<MessageManager> msgManager) override;

		void resetAnimation() override {}
		void onAnimationEnd() override {}
		
	private:
		size_t m_CurrentIndex{ 0 };
	};
}
