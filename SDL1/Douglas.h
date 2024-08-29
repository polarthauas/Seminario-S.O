/*
	--------------
	Made by Thauan
	--------------

	Gerencia o personagem principal Douglas""""

	Após o rework da física a classe ficou bem melhor!

*/

#pragma once

#include <vector>
#include <SDl2/SDL.h>
#include <memory>
#include <string>

#define DOUGLAS_WALKING "douglasWalking"
#define DOUGLAS_IDLE "douglasIdle"

class TextureMngr;
class RigidBody;

static const float DOUGLAS_SPEED = 80.0f;
static const int8_t DIRECTION_UP = 1;
static const int8_t DIRECTION_DOWN = -1;
static const int8_t DIRECTION_RIGHT = 1;
static const int8_t DIRECTION_LEFT = -1;

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
	Douglas(std::shared_ptr<TextureMngr> texturemngr);
	
	~Douglas();

	void Event(const SDL_Event& e);

	void Update(float dt);

	void Render();

	void setState(DouglasState newState);

	inline DouglasState GetState() const { return m_DouglasState; };
	inline SDL_Rect GetRect() const { return douglasRect; };
	RigidBody* getRigidBody() const;


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
	
	// Set para a variavel que determina se o jogador pode se mover ou não
	void setCanControl(bool b);

private:
	// PRECISA DE REWORK URGENTE!

	std::string idTex{ DOUGLAS_IDLE };

	DouglasState m_DouglasState;
	
	std::unique_ptr<RigidBody> m_RigidBody;

	SDL_Rect douglasRect;

	int currentFrame{ 0 };
	int frameTime = 100;
	int lastFrameTime{ 0 };

	bool isJumping{ false };

	float jumpTime{ 0.0f };
	float jumpForce{ 25 };

	bool canControl{ true };

	uint32_t jumpStartTime{ 0 };
	uint32_t lastUpdateTime{ 0 };

	std::shared_ptr<TextureMngr> m_TextureMngr;

	void loadTextures();

	void freeTexture();
};
