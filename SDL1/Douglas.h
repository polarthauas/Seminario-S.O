/*

	Classe pensada para gerenciar a fisica, comportamento e animação do Douglas

	Namoral, tem que dar rework nisso urgente kskssk
*/

#pragma once

#include <vector>
#include <SDl2/SDL.h>

// Enum para os estados do jogador
enum class DouglasState
{
	WALKING_RIGHT,
	WALKING_LEFT,
	IDLE_RIGHT,
	IDLE_LEFT,
	LOOKING_FORWARD
};


class Douglas {
public:
	explicit Douglas(SDL_Renderer* rend);
	
	~Douglas();

	void Render();

	void Update();
	void setState(DouglasState newState);

	void jump();

	inline DouglasState GetState() const { return m_DouglasState; };
	inline SDL_Rect GetRect() const { return douglasRect; };

	// Sinceramente n sei pra q essas duas funções tão aqui, mas tão
	void stopMovementX();
	void stopMovementY();

	// Ok, pra que? Sla
	void setGravity(float g);

	// Essa sim tem utilidade
	void setPosition(int x, int y);
	
	/**
	* Movimenta o jogador para um x específico
	*
	* @param dx Sla pra que isso, nmrl, é meío inutil
	* 
	* @param x - Determina para onde o jogador deve ir
	* 
	* @return retorna true se chegou no x determinado, false se não
	*/
	bool moveTo(int8_t dx, int x);
	
	// Verificador de eventos (sério, pq n ta dentro do update?)
	void Event(const SDL_Event& e);

	// Set para a variavel que determina se o jogador pode se mover ou não
	void setCanControl(bool b);

private:
	// PRECISA DE REWORK URGENTE!

	DouglasState m_DouglasState;

	SDL_Renderer* m_Cntx;

	std::vector<SDL_Texture*> sprites;

	SDL_Rect douglasRect;

	int currentFrame{ 0 };
	int frameTime = 100;
	int lastFrameTime{ 0 };
	int lastMoveTime{ 0 };

	bool isJumping{ false };

	int jumpDuration{ 0 };
	int maxJumpDuration{ 5000 };

	float speed = 2.1f;
	float jumpSpeed{ 1.5f };

	float gravity{ 9.98f };
	float maxFallSpeed{ 22.0f };
	float verticalSpeed{ 0 };

	bool canControl{ true };

	uint32_t jumpStartTime{ 0 };
	uint32_t lastUpdateTime{ 0 };

	int8_t input_x{ 0 };

	void loadTextures();

	void freeTexture();

	void applyGravity();
};
