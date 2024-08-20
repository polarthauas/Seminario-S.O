#pragma once

#include <map>
#include <string>
#include <vector>
#include <SDl2/SDL.h>

#include "Douglas.h"

class Fases
{
public:
	explicit Fases(SDL_Renderer* rend);
	~Fases();

	void Render(SDL_Renderer* rend, Douglas* douglas);

	void Collisor(const std::vector<SDL_Rect>& solids, Douglas* douglas) const;

	void loadMap(int numFase); 

private:
	std::vector<std::vector<int> > map;

	std::map<int, SDL_Texture*> texturesMap;
	std::map<int, std::string> m_TexturesPathMap;

	void freeTextures();
};
