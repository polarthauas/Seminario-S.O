#include "TextAnimLetterByLetter.h"

#include "SoundMngr.h"

void Text::LetterByLetter::startAnimation()
{
	m_CurrentIndex = 0;
	b_LastUpdateTime = SDL_GetTicks();
}
	
void Text::LetterByLetter::update()
{
    auto _currentTime = SDL_GetTicks();
    if (_currentTime - b_LastUpdateTime >= b_Delay) {
        b_LastUpdateTime = _currentTime;

        if (m_CurrentIndex < b_Text.size()) {
            if (m_CurrentIndex % 3 == 0) {
                playSound("Audio/DialogTextSoundEffects/Retro_01/Retro_Single_v2_wav.wav");
            }
            ++m_CurrentIndex;
        }
    }
}

void Text::LetterByLetter::draw(SDL_Renderer* rend, TTF_Font* font)
{   
	auto _visibleText = b_Text.substr(0, m_CurrentIndex);
    
    render(rend, font, _visibleText);
}
