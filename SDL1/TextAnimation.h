#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>

enum TextAnimTypes {
	TEXT_ANIMATION_LETTERBYLETTER,
};

class MessageManager;
class SoundMngr;

namespace Text {

	class TextAnimation {
	public:
		virtual ~TextAnimation() = default;

		TextAnimation(const std::string& text, SDL_Color color, int x, int y, int32_t delay, bool square = false, bool texture = false)
			: b_Text(text), b_Color(color), b_PosX(x), b_PosY(y), b_Delay(delay), b_Square(square), b_HasTexture(texture) {}	

		virtual void startAnimation() = 0;

		virtual void update(std::shared_ptr<SoundMngr> soundMngr) = 0;

		virtual void render(SDL_Renderer* rend, std::shared_ptr<MessageManager> messageMngr) = 0;

		virtual void resetAnimation() = 0;

	protected:

		virtual void onAnimationEnd() = 0;

		bool run = false;
		bool canRender = false;

		bool b_Square = false;
		bool b_HasTexture = false;

		uint32_t b_Delay;
		std::string b_Text{ "" };
		int b_PosX{ 0 };
		int b_PosY{ 0 };
		SDL_Color b_Color;
		uint32_t b_LastUpdateTime{ 0 };
	};
}
