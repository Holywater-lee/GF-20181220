#include "Enemy.h"
#include "ScoreManager.h"
#include "Game.h"
#include "Collision.h"
#include "MapManager.h"
#include "Audio.h"

#include "WIDTHHEIGHT.h"

Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams), playerPosition(0, 0)
{
	tag = "Enemy";
	life = 3;
	flip = SDL_FLIP_NONE;
}

void Enemy::draw()
{
	SDLGameObject::draw();
}

void Enemy::OnHit()
{
	if (currentState != EnemyState::DEAD)
	{
		life--;

		if (life <= 0)
		{
			TheScore::Instance()->AddScore(10);
			ChangeState(EnemyState::DEAD);
		}
		else
		{
			ChangeState(EnemyState::DAMAGED);
		}
	}
}

void Enemy::update()
{
	if (currentState != EnemyState::DEAD)
	{
		CheckPlayerInRange();
		CheckMoveDirection();
	}

	UpdateInState();
	SDLGameObject::update();
	CheckCollision();
}

void Enemy::UpdateInState()
{
	switch (currentPatrolState)
	{
	case PatrolState::PATROL:
		break;
	case PatrolState::CHASING:
		break;
	default:
		break;
	}

	switch (currentState)
	{
	case EnemyState::IDLE:
		m_currentRow = 0;
		m_currentFrame = (SDL_GetTicks() / 100) % 4;
		break;
	case EnemyState::MOVE:
		m_currentRow = 2;
		m_currentFrame = (SDL_GetTicks() / 100) % 4;

		Move();
		break;
	case EnemyState::ATTACK:
		switch ((SDL_GetTicks() - attackStartTime) / 100)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			m_currentRow = 1;
			m_currentFrame = 0;
			break;
		case 4:
			if (!attackFlag) Attack();
		case 5:
		case 6:
		case 7:
			m_currentFrame = 1;
			break;
		case 8:
		default:
			ChangeState(EnemyState::IDLE);
		}
		break;
	case EnemyState::DEAD:
		m_currentRow = 3;
		m_currentFrame = ((SDL_GetTicks() - deadTime) / 100) % 5;
		if (m_currentFrame == 4)
		{
			this->clean();
		}
		break;
	case EnemyState::DAMAGED:
		m_currentRow = 1;
		m_currentFrame = 2;
		if (SDL_GetTicks() >= damagedTime + knockbackTime)
		{
			ChangeState(EnemyState::IDLE);
		}
		break;
	default:
		break;
	}
}

void Enemy::ChangeState(EnemyState state)
{
	switch (currentState)
	{
	case EnemyState::IDLE:
		break;
	case EnemyState::MOVE:
		break;
	case EnemyState::ATTACK:
		attackFlag = false;
		break;
	case EnemyState::DEAD:
		break;
	case EnemyState::DAMAGED:
		std::cout << "대미지드 상태 끝" << std::endl;
		m_velocity.setX(0);
		m_velocity.setY(0);
		break;
	default:
		break;
	}

	currentState = state;

	switch (state)
	{
	case EnemyState::IDLE:
		Flipping();
		m_velocity.setX(0);
		break;
	case EnemyState::MOVE:
		break;
	case EnemyState::ATTACK:
		attackStartTime = SDL_GetTicks();
		m_velocity.setX(0);
		break;
	case EnemyState::DEAD:
		deadTime = SDL_GetTicks();
		m_velocity.setX(0);
		break;
	case EnemyState::DAMAGED:
		std::cout << "대미지드 상태 진입" << std::endl;
		Knockback();
		damagedTime = SDL_GetTicks();
		break;
	default:
		break;
	}
}

void Enemy::ChangePartrolState(PatrolState state)
{
	switch (currentPatrolState)
	{
	case PatrolState::PATROL:
		break;
	case PatrolState::CHASING:
		break;
	default:
		break;
	}

	currentPatrolState = state;

	switch (state)
	{
	case PatrolState::PATROL:
		break;
	case PatrolState::CHASING:
		if (currentState == EnemyState::IDLE)
		{
			ChangeState(EnemyState::MOVE);
		}
		break;
	default:
		break;
	}
}

void Enemy::CheckPlayerInRange()
{
	if ((SDL_GetTicks() / 100) % 5 == 0)
	{
		playerPosition = TheGame::Instance()->GetPlayerPos();
		CheckPlayerInAttackRange();

		// 거리의 제곱 계산
		if (Vector2D::LengthSquare(playerPosition, m_position) <= chasingDetectRange * chasingDetectRange)
		{
			ChangePartrolState(PatrolState::CHASING);
		}
		else
		{
			ChangePartrolState(PatrolState::PATROL);
		}

		if (currentState != EnemyState::ATTACK)
		{
			Flipping();
		}
	}
	else return;
}

void Enemy::CheckPlayerInAttackRange()
{
	// 거리의 제곱 계산
	if (Vector2D::LengthSquare(playerPosition, m_position) <= attackRange * attackRange)
	{
		if (currentState != EnemyState::ATTACK)
			ChangeState(EnemyState::ATTACK);
	}
	else return;
}

void Enemy::CheckMoveDirection()
{
	if ((SDL_GetTicks() / 100) % randomWaitTime == 0)
	{
		if (!moveDirRefreshedFlag)
		{
			moveDirRefreshedFlag = true;
			randomWaitTime = TheGame::Instance()->GetRandomInt(5, 7);
			randomDirection = TheGame::Instance()->GetRandomInt(0, 2);
		}

		switch (currentPatrolState)
		{
		case PatrolState::PATROL:
			switch (randomDirection)
			{
			case 0:
				moveDir = moveDirection::LEFT;
				break;
			case 1:
				moveDir = moveDirection::STOP;
				break;
			case 2:
				moveDir = moveDirection::RIGHT;
				break;
			default:
				break;
			}
			ChangeState(EnemyState::MOVE);

			break;
		case PatrolState::CHASING:
			if (playerPosition.getX() - m_position.getX() > 0)
			{
				moveDir = moveDirection::RIGHT;
			}
			else
			{
				moveDir = moveDirection::LEFT;
			}
			break;
		default:
			break;
		}
	}
	else if ((SDL_GetTicks() / 100) % randomWaitTime == 1)
	{
		moveDirRefreshedFlag = false;
	}

	//std::cout << "상태: " << static_cast<int>(currentState) << ", moveDir: " << static_cast<int>(moveDir) << std::endl;

}

void Enemy::Attack()
{
	attackFlag = true;
	
	TheAudio::Instance()->PlaySFX("Hit");
	attackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 16 : -16 - attackArea.w);
	attackArea.y = m_position.getY();
	for (const auto& player : TheGame::Instance()->GetGameObjects())
	{
		if (Collision::onCollision(&attackArea, player))
		{
			if (dynamic_cast<SDLGameObject*>(player)->GetTag() == "Player")
			{
				player->OnHit();
			}
		}
	}
}

void Enemy::Move()
{
	int yPos = m_position.getY() + m_height + 4;
	int xPos = m_position.getX();

	switch (moveDir)
	{
	case moveDirection::RIGHT:
		if (TheMap::Instance()->IsTileThere((xPos + m_width + 2) / TILE_SIZE, yPos / TILE_SIZE))
		{
			m_velocity.setX(moveSpeed);
		}
		else
		{
			m_velocity.setX(0);
			moveDir = moveDirection::LEFT;
		}
		flip = SDL_FLIP_NONE;
		break;
	case moveDirection::LEFT:
		if (TheMap::Instance()->IsTileThere((xPos - 2) / TILE_SIZE, yPos / TILE_SIZE))
		{
			m_velocity.setX(-moveSpeed);
		}
		else
		{
			m_velocity.setX(0);
			moveDir = moveDirection::RIGHT;
		}
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case moveDirection::STOP:
		ChangeState(EnemyState::IDLE);
		break;
	default:
		break;
	}
	/*
	if (playerPosition.getX() - m_position.getX() > 0)
	{
		if (TheMap::Instance()->IsTileThere((xPos + m_width + 2) / TILE_SIZE, yPos / TILE_SIZE))
		{
			m_velocity.setX(applySpeed);
		}
		else
		{
			m_velocity.setX(0);
		}
	}
	else
	{
		if (TheMap::Instance()->IsTileThere((xPos - 2) / TILE_SIZE, yPos / TILE_SIZE))
		{
			m_velocity.setX(-applySpeed);
		}
		else
		{
			m_velocity.setX(0);
		}
	}
	*/
}

void Enemy::Knockback()
{
	m_velocity.setX((flip == SDL_FLIP_NONE ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
	std::cout << "넉백.." << std::endl;
}

void Enemy::Flipping()
{
	if (playerPosition.getX() - m_position.getX() > 0)
	{
		flip = SDL_FLIP_NONE;
	}
	else
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
}

// 플레이어에 사용하기에는 부적합하여 Enemy에 적용 (대각선 충돌 시 이슈)
void Enemy::CheckCollision()
{
	if (!isGrounded)
	{
		m_acceleration.setY(GRAVITY);
	}

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

					m_position.setX(oldX);
					m_position.setY(tile->GetPos().getY() - m_height);
				}
				else if (m_velocity.getY() < 0 && m_position.getY() < tile->GetPos().getY() + tile->GetHeight() && m_position.getY() > tile->GetHeight())
				{
					m_position.setX(oldX);
					m_position.setY(tile->GetPos().getY() + tile->GetHeight());
					m_velocity.setY(0);
				}

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
	}
}

void Enemy::clean()
{
	SDLGameObject::clean();
}