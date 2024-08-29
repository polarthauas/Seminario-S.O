#include "MessageManager.h"

#include "Message.h"
#include "DialogBox.h"
#include "Globals.h"

MessageManager::~MessageManager()
{

    while (!messageQueue.empty()) {
        messageQueue.pop();
    }

    if (m_Font != nullptr) {
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
    }
}

bool MessageManager::setFont(const std::string& fontPath, int fontSize)
{
	if (m_Font) {
		TTF_CloseFont(m_Font);
	}

	m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);

    m_FontSize = fontSize;
    
    return m_Font != nullptr;
}

void MessageManager::render(SDL_Renderer* rend, const std::vector<std::string>& messages, const SDL_Color color, int x, int y)
{
    auto X = Global::resizeValue(x, Global::RESIZE_MODE_WIDTH);
    auto Y = Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT);

    int lineHeight = TTF_FontLineSkip(m_Font);
    int currentY = Y;

    // Renderizar cada linha
    for (const auto& l : messages) {
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

void MessageManager::render(SDL_Renderer* rend, const std::string& message, const SDL_Color color, int x, int y)
{
    auto X = Global::resizeValue(x, Global::RESIZE_MODE_WIDTH);
    auto Y = Global::resizeValue(y, Global::RESIZE_MODE_HEIGHT);

    int lineHeight = TTF_FontLineSkip(m_Font);

    auto tmp_Surface = TTF_RenderText_Blended(m_Font, message.c_str(), color);

    auto texture = SDL_CreateTextureFromSurface(rend, tmp_Surface);

    SDL_Rect destRect = { X, Y, tmp_Surface->w, tmp_Surface->h };

    SDL_RenderCopy(rend, texture, nullptr, &destRect);

    SDL_FreeSurface(tmp_Surface);
    SDL_DestroyTexture(texture);
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

void MessageManager::addMessage(std::unique_ptr<Text::Message> msg)
{
    messageQueue.push(std::move(msg));
}

void MessageManager::addDialogBox(std::unique_ptr<Text::Message> msg, const std::string& dialogBoxPath)
{
    auto _dialogBox = std::make_unique<Text::DialogBox>(std::move(msg), dialogBoxPath, m_FontSize);

    messageQueue.push(std::move(_dialogBox));
}

void MessageManager::setFontSize(uint16_t size) {
    TTF_SetFontSize(m_Font, size);
    m_FontSize = size;
}
