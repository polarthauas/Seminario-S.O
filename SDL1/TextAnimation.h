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

		TextAnimation(const std::vector<std::string>& texts, SDL_Color color, int x, int y, int32_t delay, MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE)
			: Message(texts, color, x, y, msgPositionType, 0), b_Delay(delay) {}

		virtual void startAnimation() = 0;

		virtual void update() = 0;

		virtual void draw(SDL_Renderer* rend, TTF_Font* font) = 0;

	protected:
		virtual void onAnimationEnd() = 0;

		uint32_t b_Delay;
		uint32_t b_LastUpdateTime{ 0 };
	};
}
