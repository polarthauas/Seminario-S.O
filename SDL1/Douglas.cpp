#include "Douglas.h"
#include "Globals.h"
#include "TextureMngr.h"

#include <string>
#include <SDL2/SDL_image.h>

Douglas::Douglas(SDL_Renderer* rend, std::shared_ptr<TextureMngr> texturemngr )
	: m_DouglasState(DouglasState::IDLE_RIGHT), m_Cntx(rend), m_TextureMngr(texturemngr)
{
	loadTextures();

	douglasRect = { 200, 280, Global::douglasWidth, Global::douglasHeight };
}

Douglas::~Douglas()
{
	freeTexture();
}


// Funções de set

void Douglas::setCanControl(bool b) {
	canControl = b;
}

void Douglas::setState(DouglasState newState)
{
	if (m_DouglasState != newState) {
		m_DouglasState = newState;
		currentFrame = 0;
		lastFrameTime = SDL_GetTicks();
		
		switch (m_DouglasState)
		{
		case DouglasState::WALKING_RIGHT:
		case DouglasState::WALKING_LEFT:
			idTex = DOUGLAS_WALKING;
			break;
		case DouglasState::IDLE_RIGHT:
		case DouglasState::IDLE_LEFT:
			idTex = DOUGLAS_IDLE;
			break;
		case DouglasState::LOOKING_FORWARD:
			break;
		default:
			break;
		}
	}
}

void Douglas::setPosition(int x, int y)
{
	douglasRect.x = x;
	douglasRect.y = y;
}

void Douglas::setGravity(float g)
{
	gravity = g;
}

bool Douglas::moveTo(int8_t dx, int x)
{
	input_x = dx;
	douglasRect.x += static_cast<int>(speed * input_x);
	if (douglasRect.x < x) {
		input_x = 0;
		dx == 1 ? setState(DouglasState::WALKING_RIGHT) : setState(DouglasState::WALKING_LEFT);
		return false;

	} else {
		setState(DouglasState::LOOKING_FORWARD);
		return true;
	}
}

void Douglas::loadTextures()
{
	m_TextureMngr->loadTex(DOUGLAS_WALKING, "../imgs/Douglas/Walking.png");
	m_TextureMngr->loadTex(DOUGLAS_IDLE, "../imgs/Douglas/Idle.png");
}

// A FÍSICA TA UMA MERDAAAAA

void Douglas::Update()
{
	applyGravity();

	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastFrameTime + frameTime) {
		int spriteCount = 0;

		switch (m_DouglasState) {
		case DouglasState::WALKING_LEFT:
		case DouglasState::WALKING_RIGHT:
			spriteCount = 4;  
			break;
		case DouglasState::IDLE_LEFT:
		case DouglasState::IDLE_RIGHT:
			spriteCount = 2; 
			break;
		case DouglasState::LOOKING_FORWARD:
			spriteCount = 2; 
			break;
		default:
			break;
		}

		// Atualize o frame atual com base no número de sprites disponíveis
		currentFrame = (currentFrame + 1) % spriteCount;
		lastFrameTime = currentTime;
	}

	if (input_x == 1) douglasRect.x += speed;
	else if (input_x) douglasRect.x -= speed;

	douglasRect.y += verticalSpeed;
}

void Douglas::stopMovementX()
{
	input_x = 0;
}

void Douglas::stopMovementY()
{
	verticalSpeed = 0;
}

void Douglas::Event(const SDL_Event& e) {
	if (!canControl) {
		return;
	}

	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			if (!isJumping) jump();
			break;
		case SDLK_d:
			input_x = 1;
			setState(DouglasState::WALKING_RIGHT);
			break;
		case SDLK_a:
			input_x = -1;
			setState(DouglasState::WALKING_LEFT);
			break;
		default:
			break;
		}
	}
	else if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			input_x = 0;
			setState(DouglasState::IDLE_LEFT);
			break;
		case SDLK_d:
			input_x = 0;
			setState(DouglasState::IDLE_RIGHT);
			break;
		default:
			break;
		}
	}
}

void Douglas::Render()
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (m_DouglasState == DouglasState::WALKING_LEFT || m_DouglasState == DouglasState::IDLE_LEFT) {
		flip = SDL_FLIP_HORIZONTAL;
	}

	m_TextureMngr->drawFrame(idTex, douglasRect, currentFrame, 0, 12, 14, flip);
}

void Douglas::freeTexture()
{
	for (auto& p : sprites)
		SDL_DestroyTexture(p);

	sprites.clear();
}

void Douglas::applyGravity() {
	if (isJumping) {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - jumpStartTime >= jumpDuration) {
			isJumping = false;
		}
	}
	else {
		verticalSpeed += gravity;
	}

	if (verticalSpeed > maxFallSpeed) {
		verticalSpeed = maxFallSpeed;
	}
}

void Douglas::jump() {
	if (!isJumping) {
		isJumping = true;
		verticalSpeed = -jumpSpeed;
		jumpStartTime = SDL_GetTicks();
	}
}
