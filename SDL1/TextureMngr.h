#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include <unordered_map>
#include "Texture.h"

class TextureMngr {
public:
	TextureMngr(SDL_Renderer* rend)
		:m_Rend(rend) {}

	void loadTex(const std::string& id, const std::string& path) {
		if (m_Textures.find(id) != m_Textures.end()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Textura com mesmo ID ja existe");
			return;
		}

		auto texture = std::make_unique<Texture>(m_Rend, path);

		m_Textures[id] = std::move(texture);
	}

	void drawFrame(const std::string& id, SDL_Rect& rect, int frame, int row, int sizeX, int sizeY,
		const SDL_RendererFlip flip = SDL_FLIP_NONE, const double angle = 0U) {

		SDL_Rect src = {frame*sizeX, row*sizeY, sizeX, sizeY};
		
		m_Textures[id]->draw(m_Rend, &src, &rect, angle, nullptr, flip);
	}

	void draw(const std::string& id, SDL_Rect* srcRect, SDL_Rect* dstRect, const double angle = 0,
		SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		m_Textures[id]->draw(m_Rend, srcRect, dstRect, angle, center, flip);
	}

	void dropTex(const std::string& id) {
		if (m_Textures.find(id) == m_Textures.end()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Textura não existe!");
			return;
		}

		m_Textures.erase(id);	
	}

	void clean() {
		m_Textures.clear();
	}

private:
	SDL_Renderer* m_Rend;
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;
};
