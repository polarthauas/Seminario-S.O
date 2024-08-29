#pragma once

#include "Globals.h"

class Texture {
public:
	Texture(SDL_Renderer* rend, const std::string& file) {
		loadFromFile(rend, file);
	}

	Texture(SDL_Texture* tex)
		:_tex(tex) {}
	
	~Texture() {
		clean();
	}

	void loadFromFile(SDL_Renderer* rend, const std::string& file) {
		_tex = IMG_LoadTexture(rend, file.c_str());

		if (_tex == nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erro ao carregar texture: %s, \n%s", file.c_str(), IMG_GetError());
			return;
		}

	}

	void draw(SDL_Renderer* rend, SDL_Rect* srcRect, SDL_Rect* dstRect = nullptr, const double angle = 0,
		SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE ) {
		
		if (SDL_RenderCopyEx(rend, _tex, srcRect, dstRect, angle, center, flip) != 0) {
			SDL_Log("ERROOOO");
		}
	}

	void clean() {
		SDL_DestroyTexture(_tex);
	}

private:
	SDL_Texture* _tex;
};
