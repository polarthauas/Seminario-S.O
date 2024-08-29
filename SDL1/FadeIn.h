/*

    --------------
    Made by Thauan
    --------------

    Animação top para se usar, o texto surge de pouco em poucos, aumentando seu alpha (RGBA)

    Não foi muito usado, mas ta ai ksks
*/


#pragma once

#include "TextAnimation.h"

namespace Text {
	class FadeIn : public TextAnimation {
    public:
        FadeIn(const std::vector<std::string>& texts, const SDL_Color color, int x, int y,
            MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE)
            : TextAnimation(texts, color, x, y, 50, msgpostype), m_Alpha(0) {

            startAnimation();
        }

        FadeIn(const std::string& text, const SDL_Color color, int x, int y, MessageRePositionType msgpostype = MESSAGE_REPOSITION_NONE)
            : TextAnimation(std::vector<std::string>{ text }, color, x, y, 50, msgpostype) {
            startAnimation();
        }

        void update() override;
        void draw(SDL_Renderer* rend, TTF_Font* font) override;

        void startAnimation() override;

    private:
        void onAnimationEnd() override;

        uint64_t m_Alpha{ 0 };

	};
}
