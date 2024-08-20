#include "Douglas.h"
#include "globals.h"

#include <string>
#include <SDL2/SDL_image.h>

Douglas::Douglas(SDL_Renderer* rend)
	: m_DouglasState(DouglasState::IDLE_RIGHT), m_Cntx(rend)
{
	loadTextures();

	douglasRect = { calcAlterWindowSize(200, 'w'), calcAlterWindowSize(280, 'h'), calcAlterWindowSize(DOUGLAS_WIDTH, 'w'),
		calcAlterWindowSize(DOUGLAS_HEIGHT, 'h') };
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

		loadTextures();
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
	freeTexture();

	std::string filename;
	SDL_Texture* texture;
	switch (m_DouglasState) {
	case DouglasState::WALKING_LEFT:
	case DouglasState::WALKING_RIGHT:
		for (int i = 0; i < 3; i++) {
			filename = "../imgs/Douglas/" + std::to_string(i) + ".png";

			texture = IMG_LoadTexture(m_Cntx, filename.c_str());
			sprites.push_back(texture);
		}
		break;
	case DouglasState::IDLE_LEFT:
	case DouglasState::IDLE_RIGHT:
		for (int i = 3; i < 6; i++) {
			filename = "../imgs/Douglas/" + std::to_string(i) + ".png";

			texture = IMG_LoadTexture(m_Cntx, filename.c_str());
			sprites.push_back(texture);
		}
		break;
	case DouglasState::LOOKING_FORWARD:
		for (int i = 6; i < 8; i++) {
			filename = "../imgs/Douglas/" + std::to_string(i) + ".png";

			texture = IMG_LoadTexture(m_Cntx, filename.c_str());
			sprites.push_back(texture);
		}
		break;

	default:
		break;
	}
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
			spriteCount = 3;  // Espera-se 3 sprites para WALKING_LEFT e WALKING_RIGHT
			break;
		case DouglasState::IDLE_LEFT:
		case DouglasState::IDLE_RIGHT:
			spriteCount = 3;  // Espera-se 3 sprites para IDLE_LEFT e IDLE_RIGHT
			break;
		case DouglasState::LOOKING_FORWARD:
			spriteCount = 2;  // Espera-se 2 sprites para LOOKING_FORWARD
			break;
		default:
			break;
		}

		if (spriteCount > 0 && !sprites.empty()) {
			// Atualize o frame atual com base no número de sprites disponíveis
			if (sprites.size() >= spriteCount) {
				currentFrame = (currentFrame + 1) % spriteCount;
			}
			lastFrameTime = currentTime;
		}
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

	if (SDL_RenderCopyEx(m_Cntx, sprites[currentFrame], nullptr, &douglasRect, 0, nullptr, flip) != 0) {
		SDL_Log(SDL_GetError());
	}
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
