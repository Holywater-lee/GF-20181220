#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "Collision.h"
#include "Camera.h"
#include "FXAnimation.h"

#include "WIDTHHEIGHT.h"

const int MOVESPEED = 3;
const int JUMPHEIGHT = 8;

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Player";
	life = 5;
}

void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update()
{
	handleInput();
	UpdateInState();

	//SDLGameObject::update();
	if (m_currentState != PlayerState::DEAD)
		CheckCollisionWithMove();

	TheCam::Instance()->Update(this);

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		m_position.setX(32 * 2);
		m_position.setY(32 * 16);

		m_acceleration.setY(0);
		m_velocity.setY(0);
	}

	if (KeyDown(SDL_SCANCODE_R))
	{
		TheGame::Instance()->RestartGame();
	}
}

void Player::OnHit()
{
	if (m_currentState != PlayerState::DEAD)
	{
		life--;

		if (life <= 0)
		{
			ChangeState(PlayerState::DEAD);
			std::cout << "사망!" << std::endl;
		}
		else
		{
			ChangeState(PlayerState::DAMAGED);
			std::cout << "타격 당함! 남은 체력: " << life << std::endl;
		}
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

void Player::Attack()
{
	if (KeyDown(SDL_SCANCODE_A))
	{
		ChangeState(PlayerState::ATTACK);
	}
}

void Player::ChangeWeapon()
{
	if (KeyDown(SDL_SCANCODE_DOWN) && SDL_GetTicks() >= nextWeaponChangeDelay)
	{
		cout << "무기 변경 시도 (테스트)" << endl;
		if (isRanged)
		{
			SetAttackStrategy(new MeleeAttackStrategy());
		}
		else
		{
			SetAttackStrategy(new RangedAttackStrategy());
		}
		
		nextWeaponChangeDelay = SDL_GetTicks() + weaponChangeDelay;
		isRanged = !isRanged;
	}
}

void Player::SetAttackStrategy(PlayerAttackStrategy* strategy)
{
	if (attackStrategy != nullptr)
	{
		delete attackStrategy;
		attackStrategy = nullptr;
		std::cout << "기존 스트래티지 지워짐: " << attackStrategy << std::endl;
	}
	attackStrategy = strategy;
	std::cout << "스트래티지 변경 완료: " << attackStrategy << std::endl;
}

void Player::UpdateInState()
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
		if (isRanged)
		{
			m_currentRow = 2;

			switch ((SDL_GetTicks() - attackStartTime) / 100)
			{
			case 0:
			case 1:
			case 2:
				m_currentFrame = 0;
				break;
			case 3:
				m_currentFrame = 1;
				if (!attackFXFlag)
				{
					attackFXFlag = true;
					AttackActionWithStrategy();
				}
				break;
			case 4:
			case 5:
				m_currentFrame = 2;
				break;
			case 6:
				m_currentFrame = 3;
				break;
			case 7:
			default:
				ChangeState(PlayerState::IDLE);
				break;
			}
		}
		else
		{
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

				if (!attackFXFlag)
				{
					attackFXFlag = true;
					AttackActionWithStrategy();
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
		}
		break;
	case PlayerState::DEAD:
		m_currentRow = 1;
		m_currentFrame = 1;

		if (KeyDown(SDL_SCANCODE_R))
		{
			TheGame::Instance()->RestartGame();
		}
		break;
	case PlayerState::DAMAGED:
		m_currentRow = 4;
		m_currentFrame = 0;
		if (SDL_GetTicks() >= damagedTime + knockbackTime)
		{
			ChangeState(PlayerState::IDLE);
		}
		break;
	default:
		break;
	}
}

void Player::CheckCollisionWithMove()
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
		attackFXFlag = false;
		break;
	case PlayerState::DAMAGED:
		m_velocity.setX(0);
		m_velocity.setY(0);
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
		attackStartTime = SDL_GetTicks();
		m_velocity.setX(0);
		if (!isRanged)
			TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX() + m_width / 2 - 16, m_position.getY() - 30, 32, 64, "FXSword"), SDL_GetTicks(), 350, 0));
		break;
	case PlayerState::DEAD:
		m_angle = -90;
		break;
	case PlayerState::DAMAGED:
		Knockback();
		damagedTime = SDL_GetTicks();
	default:
		break;
	}
}

void Player::Knockback()
{
	m_velocity.setX((flip == SDL_FLIP_NONE ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
}

bool Player::KeyDown(SDL_Scancode code)
{
	return TheInputHandler::Instance()->isKeyDown(code);
}

void Player::handleInput()
{
	switch (m_currentState)
	{
	case PlayerState::IDLE:
		Idle();
		Jump();
		Attack();
		ChangeWeapon();
		break;

	case PlayerState::MOVE:
		Move();
		Jump();
		Attack();
		ChangeWeapon();
		break;

	case PlayerState::JUMP:
		Move();
		Jump();
		ChangeWeapon();
		break;

	case PlayerState::ATTACK:
		break;

	case PlayerState::DEAD:
		break;

	case PlayerState::DAMAGED:
		break;
	default:
		break;
	}
}

void Player::clean()
{
	SDLGameObject::clean();
}