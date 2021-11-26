#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "Collision.h"
#include "Ball.h"
#include "Camera.h"

#include "WIDTHHEIGHT.h"

const int MOVESPEED = 3;
const int JUMPHEIGHT = 8;
const float GRAVITY = 0.5f;

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Player";
	nextShootDelay = 0;
	nextJumpDelay = 0;
}

void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update(float deltaTime)
{
	handleInput();
	UpdateFrame();

	CheckCollision(deltaTime);

	TheCam::Instance()->Update(this, deltaTime);

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		m_position.setY(100);
		m_position.setX(100);
		m_acceleration.setY(0);
		m_velocity.setY(0);
	}
}

void Player::Idle()
{
	if (KeyDown(SDL_SCANCODE_RIGHT) || KeyDown(SDL_SCANCODE_LEFT))
	{
		ChangeState(PlayerState::MOVE);
	}
}

void Player::Move()
{
	if (KeyDown(SDL_SCANCODE_RIGHT))
	{
		m_velocity.setX(MOVESPEED);
		flip = SDL_FLIP_NONE;
	}
	else if (KeyDown(SDL_SCANCODE_LEFT))
	{
		m_velocity.setX(-MOVESPEED);
		flip = SDL_FLIP_HORIZONTAL;
	}
	else
	{
		if (m_currentState == PlayerState::MOVE)
			ChangeState(PlayerState::IDLE);
	}
}

void Player::Jump()
{
	/*
	if (m_currentState != ST_JUMP && !isGrounded)
	  m_currentState = ST_JUMP;
	*/
	if (KeyDown(SDL_SCANCODE_UP))
	{
		if (SDL_GetTicks() >= nextJumpDelay && currentJumpCount < maxJumpCount)
		{
			nextJumpDelay = SDL_GetTicks() + multipleJumpDelay;

			m_velocity.setY(-JUMPHEIGHT);
			currentJumpCount++;
		}

		if (m_currentState != PlayerState::JUMP && !isGrounded)
			ChangeState(PlayerState::JUMP);
	}
}

void Player::Shoot()
{
	if (TheInputHandler::Instance()->getMouseButtonState(0))
	{
		if (SDL_GetTicks() >= nextShootDelay)
		{
			printf("공(총알) 생성\n");
			TheGame::Instance()->CreateBulletObject(new Ball(new LoaderParams(m_position.getX() + m_width / 2 - 4, m_position.getY() + m_height / 2 - 4, 8, 8, "Ball")));
			nextShootDelay = SDL_GetTicks() + ballShootDelay;
		}
	}
}

void Player::UpdateFrame()
{
	switch (m_currentState)
	{
	case PlayerState::IDLE:
		m_currentRow = 0;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	case PlayerState::MOVE:
		m_currentRow = 2;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	case PlayerState::JUMP:
		m_currentRow = 1;
		m_currentFrame = 2;
		break;
	default:
		break;
	}
}

// 개선 완료된 플레이어 충돌
void Player::CheckCollision(float deltaTime)
{
	if (!isGrounded)
	{
		m_acceleration.setY(GRAVITY);
	}
	else
	{
		if (m_currentState == PlayerState::JUMP)
			ChangeState(PlayerState::IDLE);
	}
	/*
		m_velocity.setX(m_velocity.getX() + m_acceleration.getX() * 0.1 * deltaTime);
		m_position.setX(m_position.getX() + m_velocity.getX() * 0.1 * deltaTime);
	*/
	m_velocity.setX(m_velocity.getX() + m_acceleration.getX());
	m_position.setX(m_position.getX() + m_velocity.getX());
	for (auto& tile : TheGame::Instance()->GetTileObjects())
	{
		if (Collision::onCollision(this, tile))
		{
			if (m_velocity.getX() > 0)
			{
				m_position.setX(tile->GetPos().getX() - m_width);
			}
			else
			{
				m_position.setX(tile->GetPos().getX() + tile->GetWidth());
			}
			m_velocity.setX(0);
		}
	}
	/*
		m_velocity.setY(m_velocity.getY() + m_acceleration.getY() * 0.1 * deltaTime);
		m_position.setY(m_position.getY() + m_velocity.getY() * 0.1 * deltaTime);
	*/
	m_velocity.setY(m_velocity.getY() + m_acceleration.getY());
	m_position.setY(m_position.getY() + m_velocity.getY());
	for (auto& tile : TheGame::Instance()->GetTileObjects())
	{
		if (Collision::onCollision(this, tile))
		{
			if (m_velocity.getY() > 0)
			{
				isGrounded = true;
				currentJumpCount = 0;
				m_position.setY(tile->GetPos().getY() - m_height);
			}
			else
			{
				m_position.setY(tile->GetPos().getY() + tile->GetHeight());
				m_velocity.setY(0);
			}

			if (m_currentState != PlayerState::JUMP)
			{
				m_acceleration.setY(0);
				m_velocity.setY(0);
			}
		}
	}

	m_position.setY(m_position.getY() + 1);
	int count = 0;
	for (auto& tile : TheGame::Instance()->GetTileObjects())
	{
		if (Collision::onCollision(this, tile))
		{
			count++;
		}
	}
	m_position.setY(m_position.getY() - 1);
	if (count == 0)
	{
		isGrounded = false;
		ChangeState(PlayerState::JUMP);
		if (currentJumpCount == 0) currentJumpCount = 1;
	}
}

void Player::ChangeState(PlayerState state)
{
	// 상태 나감
	switch (state)
	{
	case PlayerState::IDLE:
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::JUMP:
		break;

	default:
		break;
	}

	m_currentState = state;

	// 상태 진입
	switch (state)
	{
	case PlayerState::IDLE:
		m_velocity.setX(0);
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::JUMP:
		break;

	default:
		break;
	}
}

bool Player::KeyDown(SDL_Scancode code)
{
	return TheInputHandler::Instance()->isKeyDown(code);
}

void Player::handleInput()
{
	Shoot();

	switch (m_currentState)
	{
	case PlayerState::IDLE:
		Idle();
		Jump();
		break;

	case PlayerState::MOVE:
		Move();
		Jump();
		break;

	case PlayerState::JUMP:
		Move();
		Jump();
		break;

	default:
		break;
	}
}

void Player::clean()
{
	SDLGameObject::clean();
}