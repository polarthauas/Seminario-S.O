#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include <unordered_map>
#include "Texture.h"

class TextureMngr {
public:
	TextureMngr(SDL_Renderer* rend)
		: m_Rend(rend) {}

	void loadTex(const std::string& id, const std::string& path) {
		if (m_Textures.find(id) != m_Textures.end()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TextureMngr 18: Textura com ID: %s ja existe! Impossivel carrega-lo", id.c_str());
			return;
		}

		auto texture = std::make_unique<Texture>(m_Rend, path);

		m_Textures[id] = std::move(texture);
	}

	void addTexture(const std::string& id, SDL_Texture* texture) {
		if (m_Textures.find(id) != m_Textures.end()) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TextureMngr 29: Textura com o ID: %s já existe! Impossivel adiciona-lo", id.c_str());
			return;
		}

		m_Textures[id] = std::make_unique<Texture>(texture);
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
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TextureMngr 53: Textura de id: %s não existe! Impossivel removê-lo", id.c_str());
			return;
		}

		m_Textures.erase(id);	
	}

	void clean() {
		m_Textures.clear();
	}

	inline SDL_Renderer* getRenderer() const { return m_Rend; }

private:
	SDL_Renderer* m_Rend;
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;
};
