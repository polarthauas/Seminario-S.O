#include "DialogBox.h"

#include "TextAnimation.h"
#include "Globals.h"
#include "Game.h"

Text::DialogBox::DialogBox(std::unique_ptr<Message> msg, const std::string& dialogBoxPath, int fontSize, const std::string& speaker)
	: Message(), m_FontSize(fontSize), m_DialogBoxPath(dialogBoxPath), m_Speaker(speaker)
{
	m_Message = std::move(msg);

	//calculateRect(font);
	dialogRect = Global::resizeRect({ 0, Global::DESIGN_HEIGHT - 250, Global::DESIGN_WIDTH, 250 });

}

Text::DialogBox::~DialogBox()
{
	SDL_DestroyTexture(dialogRectTex);
	SDL_DestroyTexture(TextspeakerTex);
}

void Text::DialogBox::draw(SDL_Renderer* rend, TTF_Font* font)
{
	if(dialogRectTex == nullptr)
		dialogRectTex = IMG_LoadTexture(rend, "Assets/Game/dialog-box.png");
	if (speakerTex == nullptr)
		speakerTex = IMG_LoadTexture(rend, "Assets/Douglas/douglas_nunes.png");

	if (SDL_RenderCopy(rend, dialogRectTex, nullptr, &dialogRect) != 0) {
		SDL_Log("%s", SDL_GetError());
	}

	SDL_Rect _speakerRect = {dialogRect.x + 100, dialogRect.y + 95, 100, 100};

	if (SDL_RenderCopy(rend, speakerTex, nullptr, &_speakerRect) != 0) {
		SDL_Log("%s", SDL_GetError());	
	}

	if (TextspeakerTex == nullptr) {
		auto _font = TTF_OpenFont("../fonts/Minecraft_Seven_2.ttf", 20);

		auto _tmpSurface = TTF_RenderText_Shaded(_font, m_Speaker.c_str(), SDL_BLUE, {25, 25, 25, 30});

		TextspeakerTex = SDL_CreateTextureFromSurface(rend, _tmpSurface);

		SDL_FreeSurface(_tmpSurface);

		TTF_CloseFont(_font);
		 
		speakerTextRect = Global::resizeRect({ 80, dialogRect.y + 65, _tmpSurface->w, _tmpSurface->h });
	}
	
	SDL_RenderCopy(rend, TextspeakerTex, nullptr, &speakerTextRect);

	int adjustedFontSize = adjustFontSizeToFit(m_Message->getTexts(), font, dialogRect.w - _speakerRect.w - 50);

	TTF_SetFontSize(font, adjustedFontSize);

	m_Message->draw(rend, font);

	TTF_SetFontSize(font, m_FontSize);
}

void Text::DialogBox::update()
{
	m_Message->update();
}

int Text::DialogBox::adjustFontSizeToFit(const std::vector<std::string>& texts, TTF_Font* font, int maxWidth)
{
	int currentFontSize = m_FontSize;

	while (currentFontSize > 8) {  // Limite mínimo para evitar texto ilegível
		TTF_Font* testFont = TTF_OpenFont("../fonts/Minecraft_Seven_2.ttf", currentFontSize);
		bool fits = true;

		for (const auto& text : texts) {
			int textWidth = Global::getTextWidth(text, testFont);
			if (textWidth > maxWidth) {
				fits = false;
				break;
			}
		}

		TTF_CloseFont(testFont);

		if (fits) {
			break;
		}

		currentFontSize -= 2;  // Reduz o tamanho da fonte em 2 a cada iteração
	}

	return currentFontSize;
}
