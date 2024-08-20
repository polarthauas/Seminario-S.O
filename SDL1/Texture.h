#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

class Texture {
public:
	Texture(SDL_Renderer* rend, const std::string& file) {
		loadFromFile(rend, file);
	}

	~Texture() {
		clean();
	}

	void loadFromFile(SDL_Renderer* rend, const std::string& file) {
		_tex = IMG_LoadTexture(rend, file.c_str());
	}

	void draw(SDL_Renderer* rend, SDL_Rect* dstrect, SDL_Rect* srcRect = nullptr, const double angle = 0,
		SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE ) {
		
		SDL_RenderCopyEx(rend, _tex, dstrect, srcRect, angle, center, flip);
	}

	void clean() {
		SDL_DestroyTexture(_tex);
	}

private:
	SDL_Rect _texRect;
	SDL_Texture* _tex;
};
