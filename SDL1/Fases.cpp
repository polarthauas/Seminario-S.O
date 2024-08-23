#include "Fases.h"
#include "Douglas.h"
#include "Globals.h"

#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDl2/SDL_image.h>


Fases::Fases(SDL_Renderer* rend)
{
	m_TexturesPathMap[-2] = "Assets/Game/stone.png";
	m_TexturesPathMap[-1] = "Assets/Game/dirt.png";
	m_TexturesPathMap[1] = "Assets/Game/gram.png";
	m_TexturesPathMap[2] = "Assets/Game/wood.png";
	m_TexturesPathMap[3] = "Assets/Game/doorInf.png";
	m_TexturesPathMap[4] = "Assets/Game/doorSup.png";
	m_TexturesPathMap[5] = "Assets/Game/dark_oak_wood.png";
	m_TexturesPathMap[6] = "Assets/Game/computer.png";
	m_TexturesPathMap[7] = "Assets/Game/craftingTable.png";

	loadMap(1);

	for (const auto& pair : m_TexturesPathMap) {
		int key = pair.first;
		SDL_Texture* texture = IMG_LoadTexture(rend, pair.second.c_str());
		if (texture == nullptr) {
			SDL_Log(SDL_GetError());
		}
		else {
			texturesMap[key] = texture;
		}
	}
};

Fases::~Fases() {
	freeTextures();
}

void Fases::Render(SDL_Renderer* rend, Douglas* douglas)
{
	/*
		Renderiza a fase
	*/

	std::vector<SDL_Rect> solidBlocks;

	SDL_SetRenderDrawColor(rend, 135, 206, 235, 255);

	SDL_Rect backGroundRect = { 0, 0, Global::windowWidth, Global::windowHeight };

	SDL_RenderFillRect(rend, &backGroundRect);

	// renderiza o mapa

	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			SDL_Rect renderQuad = { x * Global::windowWidth / map[y].size(), y * Global::windowHeight / map.size() ,
				Global::windowWidth / map[y].size(), Global::windowHeight / map.size()};

			int32_t textureId = map[y][x];

			if (textureId == 6) {
				SDL_RenderCopy(rend, texturesMap[5], nullptr, &renderQuad);
			}

			SDL_RenderCopy(rend, texturesMap[textureId], nullptr, &renderQuad);
			
			if (textureId == 1 || textureId == 2 || textureId == -1 || textureId == -2 || textureId == 4 || textureId == 3	) {
				solidBlocks.push_back(renderQuad);
			}
		}
	}

	Collisor(solidBlocks, douglas);
}

void Fases::Collisor(const std::vector<SDL_Rect>& solids, Douglas* douglas) const
{
	bool isOnGround = false;

	SDL_Rect player = douglas->GetRect();

	for (const SDL_Rect& block : solids) {
		if (SDL_HasIntersection(&player, &block)) {
			int overlapLeft = (player.x + player.w) - block.x;
			int overlapRight = (block.x + block.w) - player.x;
			int overlapTop = (player.y + player.h) - block.y;
			int overlapBottom = (block.y + block.h) - player.y;

			int overlapX = std::min(overlapLeft, overlapRight);
			int overlapY = std::min(overlapTop, overlapBottom);

			if (overlapX < overlapY) {
				if (player.x < block.x) {
					douglas->setPosition(player.x - overlapX, player.y); // Colidiu pela direita
				}
				else {
					douglas->setPosition(player.x + overlapX, player.y); // Colidiu pela esquerda
				}
				douglas->stopMovementX();
			}
			else {
				if (player.y < block.y) {
					douglas->setPosition(player.x, player.y - overlapY); // Colidiu pela parte inferior
					douglas->stopMovementY();

					isOnGround = true;
				}
				else {
					douglas->setPosition(player.x, player.y + overlapY); // Colidiu pela parte superior
					douglas->stopMovementY();
				}
			}
		}
		else
			isOnGround = false;
		// Aplica a gravidade se não estiver no chão
		if (!isOnGround) {
			douglas->setGravity(0.5);
		}
		else {
			douglas->setGravity(0); // Desativa a gravidade quando está no chão
		}
	}
}



void Fases::loadMap(int numFase) {
	switch (numFase)
	{
	/*
		0 = NONE, 1 = GRAM, -1 = DIRT, -2 = STONE,
		5 = DARK WOOD, 3 = DOOR INF, 4 = DOOR SUP, 6 = CUMPUTER,
		7 = crafting table
	*/
	case 1:
		map = {
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 5, 5, 5, 5, 5, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 6, 5, 5, 5, 5, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 7, 5, 5, 5, 5, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{-1, -2, -1, -1, -1, -2, -1, -2, -2, -1, -1, -1, -2, -1, -1, -1},			
			{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
			{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
		};

		break;
	default:
		break;
	}
}

void Fases::freeTextures() 
{
	for (auto& pair : texturesMap) {
		SDL_DestroyTexture(pair.second);
	}

	texturesMap.clear();
}
