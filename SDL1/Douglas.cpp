#include "Douglas.h"
#include "Globals.h"
#include "TextureMngr.h"
#include "RigidBody.h"

Douglas::Douglas(std::shared_ptr<TextureMngr> texturemngr)
	: m_DouglasState(DouglasState::IDLE_RIGHT),
	m_TextureMngr(texturemngr), m_RigidBody(std::make_unique<RigidBody>())
{
	loadTextures();

	douglasRect = Global::resizeRect({ 200, 350, Global::DESIGN_DOUGLAS_WIDTH, Global::DESIGN_DOUGLAS_HEIGHT });

	m_RigidBody->position = Vector2D(static_cast<float>(douglasRect.x), static_cast<float>(douglasRect.y));
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
	if (m_DouglasState == newState) {
		return;
	}

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

RigidBody* Douglas::getRigidBody() const
{
	return m_RigidBody.get();
}

/*
* 
* DEPRECATED, USE RIGIDBODY
* 
*/

void Douglas::setPosition(int x, int y)
{
	douglasRect.x = x;
	douglasRect.y = y;
}

bool Douglas::moveTo(int8_t dx, int x)
{
	int8_t direction = douglasRect.x - x > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT;

	bool chegoudestino = direction == DIRECTION_RIGHT ? douglasRect.x < x : douglasRect.x > x;

	m_RigidBody->setVelocityX(DOUGLAS_SPEED * direction);
	if (chegoudestino) {
		m_RigidBody->setVelocityX(0);
		dx == 1 ? setState(DouglasState::WALKING_RIGHT) : setState(DouglasState::WALKING_LEFT);
		return false;

	} else {
		setState(DouglasState::LOOKING_FORWARD);
		return true;
	}
}

void Douglas::loadTextures()
{
	m_TextureMngr->loadTex(DOUGLAS_WALKING, "Assets/Douglas/Walking.png");
	m_TextureMngr->loadTex(DOUGLAS_IDLE, "Assets/Douglas/Idle.png");
}

void Douglas::Event(const SDL_Event& e) {
	if (!canControl) {
		return;
	}

	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			if (!isJumping) {
				m_RigidBody->applyForceY(jumpForce * DIRECTION_UP);
				isJumping = true;
				jumpTime = SDL_GetTicks();
			}
			break;
		case SDLK_d:
			m_RigidBody->setVelocityX(DOUGLAS_SPEED * DIRECTION_RIGHT);
			setState(DouglasState::WALKING_RIGHT);
			break;
		case SDLK_a:
			m_RigidBody->setVelocityX(DOUGLAS_SPEED * DIRECTION_LEFT);
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
			m_RigidBody->setVelocityX(0);
			setState(DouglasState::IDLE_LEFT);
			break;
		case SDLK_d:
			m_RigidBody->setVelocityX(0);
			setState(DouglasState::IDLE_RIGHT);
			break;
		default:
			break;
		}
	}
}

void Douglas::Update(float dt)
{
	//Atualiza o rigidBody + gravidade
	m_RigidBody->update(dt);

	douglasRect.x = m_RigidBody->position.x;
	douglasRect.y = m_RigidBody->position.y;
	
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastFrameTime + frameTime) {
		int spriteCount = 0;

		switch (m_DouglasState) {
		case DouglasState::WALKING_LEFT:
		case DouglasState::WALKING_RIGHT:
			spriteCount = 4;
			frameTime = 100;
			break;
		case DouglasState::IDLE_LEFT:
		case DouglasState::IDLE_RIGHT:
			spriteCount = 2;
			frameTime = 175;
			break;
		}

		// Atualize o frame atual com base no número de sprites disponíveis
		currentFrame = (currentFrame + 1) % spriteCount;
		lastFrameTime = currentTime;
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
	m_TextureMngr->dropTex(DOUGLAS_IDLE);
	m_TextureMngr->dropTex(DOUGLAS_WALKING);
}

