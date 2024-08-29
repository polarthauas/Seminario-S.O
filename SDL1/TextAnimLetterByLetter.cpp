#include "TextAnimLetterByLetter.h"

#include "SoundMngr.h"

void Text::LetterByLetter::startAnimation()
{
	m_CurrentLineIndex = 0;
	b_LastUpdateTime = SDL_GetTicks();
    updateVisibleText();
}

void Text::LetterByLetter::updateVisibleText()
{
    m_VisibleText.clear();
    // Adiciona apenas a linha atual e anteriores
    for (size_t i = 0; i < b_Texts.size() && i <= m_CurrentLineIndex; ++i) {
        std::string line = b_Texts[i].substr(0, (i == m_CurrentLineIndex) ? m_CurrentTextIndex : b_Texts[i].size());
        m_VisibleText.push_back(line);
    }
}
	
void Text::LetterByLetter::update()
{
    auto _currentTime = SDL_GetTicks();
    if (_currentTime - b_LastUpdateTime >= b_Delay) {
        b_LastUpdateTime = _currentTime;

        if (m_CurrentLineIndex >= b_Texts.size()) return;

        if (m_CurrentTextIndex < b_Texts[m_CurrentLineIndex].size()) {
            ++m_CurrentTextIndex;
            updateVisibleText();
            
            if(m_CurrentTextIndex % 2 == 0)
                playSound("Audio/DialogTextSoundEffects/Retro_01/Retro_Single_v2_wav.wav");
        }
        else {
            // Passar para a próxima linha
            ++m_CurrentLineIndex;
            m_CurrentTextIndex = 0;
            updateVisibleText();
        }
    }
}
    
void Text::LetterByLetter::draw(SDL_Renderer* rend, TTF_Font* font)
{   
    render(rend, font, m_VisibleText);
}
