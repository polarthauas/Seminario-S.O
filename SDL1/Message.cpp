#include "Message.h"
#include "Globals.h"

#include <string>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void Message::render(SDL_Renderer* rend, TTF_Font* font, std::string text)
{
    std::string _text;
    
    if (text.empty()) {
        _text = b_Text;
    }
    else {
        _text = text;
    }


    auto X = Global::resizeValue(b_X, Global::RESIZE_MODE_WIDTH);
    auto Y = Global::resizeValue(b_Y, Global::RESIZE_MODE_HEIGHT);

    std::istringstream stream(_text);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line, '\n')) {
        lines.push_back(line);
    }

    int lineHeight = TTF_FontLineSkip(font);
    int currentY = Y;

    if (b_HasSquare) {
        // Renderizar o fundo para o teXto
        int maxWidth = 0;
        for (const auto& l : lines) {
            auto tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), b_Color);
            if (tmp_Surface) {
                maxWidth = std::max(maxWidth, tmp_Surface->w);
                SDL_FreeSurface(tmp_Surface);
            }
        }

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // Branco
        SDL_Rect backgroundRect = { X - 20, Y - 20, maxWidth + 40, lineHeight * lines.size() + 30 };
        SDL_RenderFillRect(rend, &backgroundRect);
    }

    // Renderizar cada linha
    for (const auto& l : lines) {
        auto tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), b_Color);
        if (!tmp_Surface) continue;

        auto texture = SDL_CreateTextureFromSurface(rend, tmp_Surface);
       
        SDL_Rect destRect = { X, currentY, tmp_Surface->w, tmp_Surface->h };
        SDL_RenderCopy(rend, texture, nullptr, &destRect);

        SDL_FreeSurface(tmp_Surface);
        SDL_DestroyTexture(texture);

        currentY += lineHeight; // Mover para a próXima linha
    }
}
