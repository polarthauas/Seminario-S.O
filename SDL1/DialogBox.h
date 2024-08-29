#pragma once

#include <memory>
#include <string>

#include "Message.h"

namespace Text {
	class DialogBox : public Message {
	public:
		DialogBox(std::unique_ptr<Message> msg, const std::string& dialogBoxPath, int fontSize, const std::string& speaker = "Douglas:");
		~DialogBox();

		void draw(SDL_Renderer* rend, TTF_Font* font) override;

		void update() override;

	private:
		int adjustFontSizeToFit(const std::vector<std::string>& texts, TTF_Font* font, int maxWidth);

		SDL_Texture* dialogRectTex{ nullptr };
		SDL_Texture* TextspeakerTex{ nullptr };
		SDL_Texture* speakerTex{ nullptr };

		std::string m_DialogBoxPath;
		std::string m_Speaker;

		int m_FontSize;

		SDL_Rect dialogRect;
		SDL_Rect speakerTextRect;

		std::unique_ptr<Message> m_Message;
	};
}

