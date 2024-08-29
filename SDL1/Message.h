#pragma once

#include "Globals.h"
#include <array>
#include <functional>

namespace Text {
	enum MessageRePositionType {
		MESSAGE_REPOSITION_NONE,
		MESSAGE_REPOSITION_DIALOGBOX,
	};

	class Message {
	public:
		using OnDestroyCallback = std::function<void(Message*)>;

		Message() = default;
		Message(const std::vector<std::string>& text, const SDL_Color color, int x, int y, MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE, int fontsize = 0)
			:b_Texts(text), b_Color(color), msgPositionType(msgpostype), b_FontSize(fontsize) {
			msgRect.x = x;
			msgRect.y = y;
		}
	
		virtual ~Message() {
			if (b_CallBack) {
				b_CallBack(this);
			}
		}

		virtual void update() {};

		virtual void draw(SDL_Renderer* rend, TTF_Font* font) {
			render(rend, font, b_Texts);
		}

		void setOnDestroyCallback(OnDestroyCallback callback) {
			b_CallBack = callback;
		}

		void render(SDL_Renderer* rend, TTF_Font* font, const std::vector<std::string>& texts = {});

		inline std::vector<std::string> getTexts() const { return b_Texts; }
		inline std::array<int, 2> getPosition() const { return { msgRect.x, msgRect.y }; }
		inline void setPositionX(int x) { msgRect.x = x; }
		inline void setPositionY(int y) { msgRect.y = y; }
		
	protected:
		std::vector<std::string> b_Texts;
		SDL_Color b_Color;

		OnDestroyCallback b_CallBack;

		SDL_Rect msgRect = { 0, 0, 0, 0 };

		int b_FontSize;
		MessageRePositionType msgPositionType;
	};
}
