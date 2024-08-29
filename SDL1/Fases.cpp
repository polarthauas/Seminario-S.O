#include "Fases.h"
#include "Douglas.h"
#include "Globals.h"

#include <map>
#include "RigidBody.h"

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

void Fases::draw(SDL_Renderer* rend, Douglas* douglas)
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

	SDL_Rect playerRect = douglas->GetRect();
	Vector2D oldPosition = douglas->getRigidBody()->oldPosition;  // Usa oldPosition para verificar a mudança
	bool isOnGround = false;

	for (const SDL_Rect& block : solidBlocks) {
		SDL_Rect blockRect = block;

		// Converte as posições do RigidBody e do bloco para SDL_Rect
		SDL_Rect player = { static_cast<int>(playerRect.x), static_cast<int>(playerRect.y),
							playerRect.w, playerRect.h };

		if (SDL_HasIntersection(&player, &blockRect)) {
			int overlapLeft = (player.x + player.w) - blockRect.x;
			int overlapRight = (blockRect.x + blockRect.w) - player.x;
			int overlapTop = (player.y + player.h) - blockRect.y;
			int overlapBottom = (blockRect.y + blockRect.h) - player.y;

			int overlapX = std::min(overlapLeft, overlapRight);
			int overlapY = std::min(overlapTop, overlapBottom);

			if (overlapX < overlapY) {
				if (player.x < blockRect.x) {
					douglas->getRigidBody()->position.x = blockRect.x - player.w; // Colidiu pela direita
				}
				else {
					douglas->getRigidBody()->position.x = blockRect.x + blockRect.w; // Colidiu pela esquerda
				}
				douglas->getRigidBody()->velocity.x = 0; // Zera a velocidade horizontal
			}
			else {
				if (player.y < blockRect.y) {
					douglas->getRigidBody()->position.y = blockRect.y - player.h; // Colidiu pela parte inferior
					douglas->getRigidBody()->velocity.y = 0; // Zera a velocidade vertical
					isOnGround = true;
				}
				else {
					douglas->getRigidBody()->position.y = blockRect.y + blockRect.h; // Colidiu pela parte superior
					douglas->getRigidBody()->velocity.y = 0; // Zera a velocidade vertical
				}
			}
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
