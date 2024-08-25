#include "TextAnimLetterByLetter.h"

#include "MessageManager.h"
#include "SoundMngr.h"

void Text::LetterByLetter::startAnimation()
{
	m_CurrentIndex = 0;
	b_LastUpdateTime = SDL_GetTicks();
	run = true;
	canRender = true;
}

void Text::LetterByLetter::update(std::shared_ptr<SoundMngr> soundMngr)
{
    if (!run) return;

    auto _currentTime = SDL_GetTicks();
    if (_currentTime - b_LastUpdateTime >= b_Delay) {
        b_LastUpdateTime = _currentTime;


        if (m_CurrentIndex < b_Text.size()) {
            ++m_CurrentIndex;
            if(m_CurrentIndex % 2 == 0)
                soundMngr->playSound("Audio/DialogTextSoundEffects/Retro_01/Retro_Single_v2_wav.wav");
        }
        else {
            run = false;
        }
    }
}
	
void Text::LetterByLetter::render(SDL_Renderer* rend, std::shared_ptr<MessageManager> msgManager)
{
    if (!canRender) return;

	auto _visibleText = b_Text.substr(0, m_CurrentIndex);

	msgManager->render(rend, _visibleText, b_Color, b_PosX, b_PosY, b_Square, b_HasTexture);
}
