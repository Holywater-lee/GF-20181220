#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "Collision.h"
#include "Ball.h"
#include "Camera.h"
#include "FXAnimation.h"

#include "WIDTHHEIGHT.h"

const int MOVESPEED = 3;
const int JUMPHEIGHT = 8;
const float GRAVITY = 0.5f;

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Player";
}

void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update()
{
	handleInput();
	UpdateFrame();

	//SDLGameObject::update();
	CheckCollision();

	TheCam::Instance()->Update(this);

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		m_position.setX(32 * 2);
		m_position.setY(32 * 16);

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

void Player::Attack()
{
	if (KeyDown(SDL_SCANCODE_DOWN) && !isAttack)
	{
		cout << "플레이어 근접 공격 시도" << endl;
		ChangeState(PlayerState::ATTACK);
		attackStartTime = SDL_GetTicks();
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
		m_currentRow = 3;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	case PlayerState::JUMP:
		m_currentRow = 4;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	case PlayerState::ATTACK:
		m_currentRow = 1;

		switch ((SDL_GetTicks() - attackStartTime) / 100)
		{
		case 0:
		case 1:
			m_currentFrame = 0;
			break;
		case 2:
		case 3:
		case 4:
			m_currentFrame = 1;
			break;
		case 5:
			m_currentFrame = 2;

			if (!slashFXFlag)
			{
				slashFXFlag = true;
				TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? 2 : -2, 20);
				attackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
				attackArea.y = m_position.getY() + 2;
				for (const auto& enemy : TheGame::Instance()->GetGameObjects())
				{
					if (Collision::onCollision(&attackArea, enemy))
					{
						if (dynamic_cast<SDLGameObject*>(enemy)->GetTag() == "Enemy")
						{
							enemy->OnHit();
						}
					}
				}
				TheGame::Instance()->CreateFX(new FXAnimation(new LoaderParams(attackArea.x, attackArea.y, 32, 64, "FXSword"), SDL_GetTicks(), 350, 1, flip == SDL_FLIP_HORIZONTAL));
			}
			break;
		case 6:
		case 7:
			m_currentFrame = 3;
			break;
		case 8:
		default:
			ChangeState(PlayerState::IDLE);
			break;
		}
		break;
	default:
		break;
	}
}

void Player::CheckCollision()
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
	float oldX = m_position.getX();

	for (auto& tile : TheGame::Instance()->GetTileObjects())
	{
		if (Collision::onCollision(this, tile))
		{
			if (m_position.getY() != tile->GetPos().getY() + tile->GetHeight() && m_position.getY() + m_height != tile->GetPos().getY())
			{
				if (m_velocity.getX() > 0 && m_position.getX() + m_width > tile->GetPos().getX() && m_position.getX() + m_width < tile->GetPos().getX() + tile->GetWidth())
				{
					m_position.setX(tile->GetPos().getX() - m_width);
				}
				else if (m_velocity.getX() < 0 && m_position.getX() < tile->GetPos().getX() + tile->GetWidth() && m_position.getX() > tile->GetWidth())
				{
					m_position.setX(tile->GetPos().getX() + tile->GetWidth());
				}
				m_velocity.setX(0);
			}

			if (m_position.getX() != tile->GetPos().getX() + tile->GetWidth() && m_position.getX() + m_width != tile->GetPos().getX())
			{
				if (m_velocity.getY() > 0 && m_position.getY() + m_height > tile->GetPos().getY() && m_position.getY() + m_height < tile->GetPos().getY() + tile->GetHeight())
				{
					isGrounded = true;
					currentJumpCount = 0;
					m_position.setX(oldX);
					m_position.setY(tile->GetPos().getY() - m_height);
				}
				else if (m_velocity.getY() < 0 && m_position.getY() < tile->GetPos().getY() + tile->GetHeight() && m_position.getY() > tile->GetHeight())
				{
					m_position.setX(oldX);
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
	}
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
			else if (m_velocity.getX())
			{
				m_position.setX(tile->GetPos().getX() + tile->GetWidth());
			}
			m_velocity.setX(0);
		}
	}

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
			else if (m_velocity.getY() < 0)
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
	switch (m_currentState)
	{
	case PlayerState::IDLE:
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::JUMP:
		break;
	case PlayerState::ATTACK:
		isAttack = false;
		slashFXFlag = false;
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
	case PlayerState::ATTACK:
		isAttack = true;
		m_velocity.setX(0);
		TheGame::Instance()->CreateFX(new FXAnimation(new LoaderParams(m_position.getX() + m_width / 2 - 16, m_position.getY() - 30, 32, 64, "FXSword"), SDL_GetTicks(), 350, 0));
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
		Attack();
		break;

	case PlayerState::MOVE:
		Move();
		Jump();
		Attack();
		break;

	case PlayerState::JUMP:
		Move();
		Jump();
		break;

	case PlayerState::ATTACK:
		break;

	default:
		break;
	}
}

void Player::clean()
{
	SDLGameObject::clean();
}