#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>


class SoundMngr;
class MessageManager;
class ButtonMngr;
class MessageManager;
class TextureMngr;

namespace Text {
	class TextAnimMngr;
}

class Menu {
public:
	Menu(SDL_Renderer* renderer, std::shared_ptr<TextureMngr> texturemngr,
		std::shared_ptr<ButtonMngr> buttonmngr, std::shared_ptr<SoundMngr> soundmngr);

	~Menu();

	void Events(const SDL_Event& e);
	void Render();

	bool isStartClicked() const;
	bool isExitClicked() const;

private:
	SDL_Renderer* m_Renderer;

	Text::TextAnimMngr* textAnimMngr;
	
	std::shared_ptr<TextureMngr> m_TextureMngr;
	std::shared_ptr<ButtonMngr> m_ButtonMngr;
	std::shared_ptr<SoundMngr> m_SoundMngr;

	std::shared_ptr<MessageManager> msgManager;
	
	bool startClicked;
	bool exitClicked;

	void loadTextures();
	void freeTextures();
};