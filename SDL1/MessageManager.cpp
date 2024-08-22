#include "MessageManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <vector>

MessageManager::MessageManager()
	: font(nullptr)
{
}

MessageManager::~MessageManager()
{
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

bool MessageManager::setFont(const std::string& fontPath, int fontSize)
{
	if (font) {
		TTF_CloseFont(font);
	}

	font = TTF_OpenFont(fontPath.c_str(), fontSize);
    
    if (!font) return false;
    return true;

}

void MessageManager::Render(SDL_Renderer* rend, const std::string& message, SDL_Color textColor, int x, int y,  bool square, bool tex)
{
    // Dividir o texto em linhas

    auto X = Global::resizeValue(x, Global::RESIZE_MODE_WIDTH);
    auto Y = Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT);

    std::istringstream stream(message);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line, '\n')) {
        lines.push_back(line);
    }

    int lineHeight = TTF_FontLineSkip(font);
    int currentY = Y;

    if (square) {
        // Renderizar o fundo para o teXto
        int maxWidth = 0;
        for (const auto& l : lines) {
            SDL_Surface* tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), textColor);
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
        SDL_Surface* tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), textColor);
        if (!tmp_Surface) continue;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, tmp_Surface);
        if (!texture) {
            SDL_FreeSurface(tmp_Surface);
            continue;
        }

        SDL_Rect destRect = { X, currentY, tmp_Surface->w, tmp_Surface->h };
        SDL_RenderCopy(rend, texture, nullptr, &destRect);

        // Limpa recursos
        SDL_FreeSurface(tmp_Surface);
        SDL_DestroyTexture(texture);

        currentY += lineHeight; // Mover para a próXima linha
    }

    if (tex) {
        auto _tex = IMG_LoadTexture(rend, pathTex.c_str());
        auto w = Global::douglasWidth;
        auto h = Global::douglasHeight;

        SDL_Rect auxRect = { x - w - 20, Y, w, h};
        if (SDL_RenderCopy(rend, _tex, nullptr, &auxRect) < 0) {
            SDL_Log(SDL_GetError());
        }

        SDL_DestroyTexture(_tex);
    }
}

void MessageManager::setFontSize(uint16_t size) {
    TTF_SetFontSize(font, size);
}

int MessageManager::getTextWidth(const std::string & text, int fontSize) {
    TTF_SetFontSize(font, fontSize);
    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(font, text.c_str(), &textWidth, &textHeight) != 0) {
        SDL_Log("Erro ao calcular o tamanho do texto: %s", TTF_GetError());
        return 0;
    }

    return textWidth;
}