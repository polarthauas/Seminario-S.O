#include "Message.h"
#include "Globals.h"

void Text::Message::render(SDL_Renderer* rend, TTF_Font* font, const std::vector<std::string>& texts)
{
    int lineHeight = TTF_FontLineSkip(font);
    int totalHeight = lineHeight * texts.size();

    int dialogBoxY = Global::resizeValue(Global::DESIGN_HEIGHT - 250, Global::RESIZE_MODE_HEIGHT);
    int Y = dialogBoxY + (250 - totalHeight) / 2;

    int dialogBoxWidth = Global::resizeValue(Global::DESIGN_WIDTH, Global::RESIZE_MODE_WIDTH);

    for (const auto& l : texts) {
        auto tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), b_Color);
        if (!tmp_Surface) continue;

        auto texture = SDL_CreateTextureFromSurface(rend, tmp_Surface);

        int X = (dialogBoxWidth - tmp_Surface->w) / 2;

        SDL_Rect destRect = { X, Y, tmp_Surface->w, tmp_Surface->h };

        SDL_RenderCopy(rend, texture, nullptr, &destRect);

        SDL_FreeSurface(tmp_Surface);
        SDL_DestroyTexture(texture);

        Y += lineHeight; // Mover para a próxima linha
    }
}

