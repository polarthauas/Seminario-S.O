#include "Globals.h"

int Global::windowHeight;
int Global::windowWidth;

int Global::douglasHeight;
int Global::douglasWidth;

int Global::resizeValue(int v, Global::RESIZE_MODE mode)
{
	float scaleX = (windowWidth / (DESIGN_WIDTH*1.0f));
	float scaleY = (windowHeight / (DESIGN_HEIGHT * 1.0f));

	switch (mode)
	{
	case Global::RESIZE_MODE_WIDTH:
		return static_cast<int>(v * scaleX);
	case Global::RESIZE_MODE_HEIGHT:
		return static_cast<int>(v * scaleY);
	default:
		break;
	}

	return 0;
}

bool Global::mouseInRect(int x, int y, SDL_Rect& b) {
	SDL_Point p = { x, y };

	return SDL_PointInRect(&p, &b);
}

int Global::getTextWidth(const std::string& text, TTF_Font* font)
{
	int w, h;

	TTF_SizeText(font, text.c_str(), &w, &h);

    return w;
}

int Global::getTextHeight(const std::string& text, const std::string& fontPath, int fontSize)
{
    auto font = TTF_OpenFont(fontPath.c_str(), fontSize);

	if (font == nullptr) {
		SDL_Log("%s", TTF_GetError());
		return 0;
	}

    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(font, text.c_str(), &textWidth, &textHeight) != 0) {
        SDL_Log("Erro ao calcular o tamanho do texto: %s", TTF_GetError());
        return 0;
    }

    TTF_CloseFont(font);

    return textHeight;
}


SDL_Rect Global::resizeRect(SDL_Rect a)
{
	SDL_Rect _rect = {};
	_rect.x = resizeValue(a.x, RESIZE_MODE_WIDTH);
	_rect.y = resizeValue(a.y, RESIZE_MODE_HEIGHT);
	_rect.w = resizeValue(a.w, RESIZE_MODE_WIDTH);
	_rect.h = resizeValue(a.h, RESIZE_MODE_HEIGHT);

	return _rect;
}
