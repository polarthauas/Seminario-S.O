#include "MessageManager.h"
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <sstream>
#include <vector>

MessageManager::MessageManager()
	: font(nullptr)
{
	TTF_Init();
}

MessageManager::~MessageManager()
{
	if (font) TTF_CloseFont(font);

	TTF_Quit();
}

void MessageManager::setFont(const std::string& fontPath, int fontSize)
{
	if (font) {
		TTF_CloseFont(font);
	}

	font = TTF_OpenFont(fontPath.c_str(), fontSize);
}

void MessageManager::Render(SDL_Renderer* rend, const std::string& message, SDL_Color textColor, int x, int y,  bool square, bool tex)
{
    // Dividir o texto em linhas
    std::istringstream stream(message);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line, '\n')) {
        lines.push_back(line);
    }

    int lineHeight = TTF_FontLineSkip(font);
    int currentY = y;

    if (square) {
        // Renderizar o fundo para o texto
        int maxWidth = 0;
        for (const auto& l : lines) {
            SDL_Surface* tmp_Surface = TTF_RenderText_Blended(font, l.c_str(), textColor);
            if (tmp_Surface) {
                maxWidth = std::max(maxWidth, tmp_Surface->w);
                SDL_FreeSurface(tmp_Surface);
            }
        }
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // Branco
        SDL_Rect backgroundRect = { x - 20, y - 20, maxWidth + 40, lineHeight * lines.size() + 30 };
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

        SDL_Rect destRect = { x, currentY, tmp_Surface->w, tmp_Surface->h };
        SDL_RenderCopy(rend, texture, nullptr, &destRect);

        // Limpa recursos
        SDL_FreeSurface(tmp_Surface);
        SDL_DestroyTexture(texture);

        currentY += lineHeight; // Mover para a próxima linha
    }

    if (tex) {
        auto SDL_Tex = IMG_LoadTexture(rend, pathTex.c_str());
        auto w = calcAlterWindowSize(DOUGLAS_WIDTH, 'w');
        auto h = calcAlterWindowSize(DOUGLAS_HEIGHT, 'h');

        SDL_Rect auxRect = { x - w - 20, y, w, h};
        if (SDL_RenderCopy(rend, SDL_Tex, nullptr, &auxRect) < 0) {
            SDL_Log(SDL_GetError());
        }
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