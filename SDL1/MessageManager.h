#pragma once

#include <string>
#include <queue>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Message;

class MessageManager
{
public:
	MessageManager();
	~MessageManager();
	
	bool setFont(const std::string& fontPath, int fontSize);

	void render(SDL_Renderer* rend, const std::string& message, SDL_Color color, int x, int y);

	void renderAll(SDL_Renderer* rend);
	void updateAll();

	void popQueue();

	void processInput(const SDL_Event& e);

	void addMessage(std::unique_ptr<Message> msg);
	
	int getTextWidth(const std::string& text, int fontSize);

	void setImageTexture(SDL_Renderer* rend, const std::string& newPath);

	void setFontSize(uint16_t size);

private:
	std::queue<std::unique_ptr<Message>> messageQueue;

	SDL_Texture* _tex{ nullptr };
	TTF_Font* m_Font { nullptr };
};
