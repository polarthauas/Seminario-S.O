#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "Message.h"

class MessageManager;

namespace Text {

	class TextAnimation : public Message {
	public:
		virtual ~TextAnimation() = default;

		TextAnimation(const std::string& text, SDL_Color color, int x, int y, int32_t delay, bool square = false, bool texture = false)
			: Message(text, color, x, y, square, texture), b_Delay(delay) {}

		virtual void startAnimation() = 0;

		virtual void update() = 0;

		virtual void draw(SDL_Renderer* rend, TTF_Font* font) = 0;

		virtual void resetAnimation() = 0;

	protected:

		virtual void onAnimationEnd() = 0;

		bool run = false;
		bool canRender = false;

		uint32_t b_Delay;
		uint32_t b_LastUpdateTime{ 0 };
	};
}
