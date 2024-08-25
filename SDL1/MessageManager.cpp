#include "MessageManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <vector>
#include <SDL2/SDL_ttf.h>

#include "Message.h"
#include "Globals.h"

MessageManager::MessageManager() {}

MessageManager::~MessageManager()
{

    while (!messageQueue.empty()) {
        messageQueue.pop();
    }

    if (m_Font != nullptr) {
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
    }
    if (_tex != nullptr) {
        SDL_DestroyTexture(_tex);
        _tex = nullptr;
    }
}

bool MessageManager::setFont(const std::string& fontPath, int fontSize)
{
	if (m_Font) {
		TTF_CloseFont(m_Font);
	}

	m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);
    
    return m_Font != nullptr;

}

void MessageManager::render(SDL_Renderer* rend, const std::string& message, SDL_Color color, int x, int y)
{
    auto X = Global::resizeValue(x, Global::RESIZE_MODE_WIDTH);
    auto Y = Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT);

    std::istringstream stream(message); 
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line, '\n')) {
        lines.push_back(line);
    }

    int lineHeight = TTF_FontLineSkip(m_Font);
    int currentY = Y;

    // Renderizar cada linha
    for (const auto& l : lines) {
        auto tmp_Surface = TTF_RenderText_Blended(m_Font, l.c_str(), color);
        if (!tmp_Surface) continue;

        auto texture = SDL_CreateTextureFromSurface(rend, tmp_Surface);

        SDL_Rect destRect = { X, currentY, tmp_Surface->w, tmp_Surface->h };
        SDL_RenderCopy(rend, texture, nullptr, &destRect);

        SDL_FreeSurface(tmp_Surface);
        SDL_DestroyTexture(texture);

        currentY += lineHeight; // Mover para a próXima linha
    }
}

void MessageManager::renderAll(SDL_Renderer* rend)
{
    if (messageQueue.empty()) return;   

    auto& msg = messageQueue.front();
    
    msg->draw(rend, m_Font);
}

void MessageManager::updateAll()
{
    if (messageQueue.empty()) return;

    auto& msg = messageQueue.front();

    msg->update();
}

void MessageManager::popQueue()
{
    if (messageQueue.empty()) return;

    messageQueue.pop();
}

void MessageManager::processInput(const SDL_Event& e)
{
    if (messageQueue.empty()) return;

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
        messageQueue.pop();
    }
}

void MessageManager::addMessage(std::unique_ptr<Message> msg)
{
    messageQueue.push(std::move(msg));
}

void MessageManager::setFontSize(uint16_t size) {
    TTF_SetFontSize(m_Font, size);
}

int MessageManager::getTextWidth(const std::string & text, int fontSize) {
    TTF_SetFontSize(m_Font, fontSize);
    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(m_Font, text.c_str(), &textWidth, &textHeight) != 0) {
        SDL_Log("Erro ao calcular o tamanho do texto: %s", TTF_GetError());
        return 0;
    }

    return textWidth;
}

void MessageManager::setImageTexture(SDL_Renderer* rend, const std::string& newPath)
{
    _tex = IMG_LoadTexture(rend, newPath.c_str());
    if (_tex == nullptr) {
        SDL_Log("Erro: %s", IMG_GetError());
    }
}

