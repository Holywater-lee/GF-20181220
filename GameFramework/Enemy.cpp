#include "Enemy.h"
#include "ScoreManager.h"
#include "Utility.h"
#include "Game.h"
#include "Collision.h"
#include "MapManager.h"
#include "Audio.h"

#include "WIDTHHEIGHT.h"

// 초기화
Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams), playerPosition(0, 0)
{
	tag = "Enemy";
	life = 3;
	maxLife = life; 
}

void Enemy::draw()
{
	SDLGameObject::draw();
}

// 피격당하는 함수
void Enemy::OnHit(int amount)
{
	if (currentState != EnemyState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// 스코어에 10점 추가
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
	// 사망 상태가 아니면
	if (currentState != EnemyState::DEAD)
	{
		CheckPlayerInRange(); // 플레이어가 거리 내에 있는지 (Patrol, Chasing)
		CheckPlayerInAttackRange(); // 공격 거리 내에 있는지 (계속 쫓아가는지, 공격할건지)
		CheckMoveDirection(); // 랜덤 시간마다 방향 체크 (랜덤인 이유는 다른 Enemy와 겹치지 않게끔 + 생동감?)
	}

	UpdateInState();
	SDLGameObject::update();
	CheckCollision();

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		this->clean();
	}
}

// 상태별 다른 update
void Enemy::UpdateInState()
{
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

// 상태를 변경해주는 함수
void Enemy::ChangeState(EnemyState state)
{
	// 상태에서 나갈 때 처리
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
		m_velocity.setX(0);
		m_velocity.setY(0);
		break;
	default:
		break;
	}

	// 상태 변경
	currentState = state;

	// 변경된 상태에 대한 처리
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
		damagedTime = SDL_GetTicks();
		Knockback();
		break;
	default:
		break;
	}
}

// 패트롤 상태를 변경하는 함수
void Enemy::ChangePatrolState(PatrolState state)
{
	// 패트롤 상태 변경
	currentPatrolState = state;

	// 변경된 패트롤 상태에 대한 처리
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

// 플레이어가 추적거리 내에 있는지 확인하는 함수
void Enemy::CheckPlayerInRange()
{
	// 일정 주기마다 실행
	if ((SDL_GetTicks() / 100) % 5 == 0)
	{
		// 플레이어 위치값을 받아옴
		playerPosition = TheGame::Instance()->GetPlayerPos();
		//playerPosition = TheGame::Instance()->FindObjectPositionWithTag("Player");

		// 거리의 제곱 계산 (제곱근으로 거리를 계산하는 것보다 빠름), LengthSquare은 직접 추가한 Vector2D 함수
		if (Vector2D::LengthSquare(playerPosition, m_position) <= chasingDetectRange * chasingDetectRange)
		{
			ChangePatrolState(PatrolState::CHASING);
		}
		else
		{
			ChangePatrolState(PatrolState::PATROL);
		}

		// 공격중이 아니라면 움직임 방향에 따라 flip 변경
		if (currentState != EnemyState::ATTACK)
		{
			Flipping();
		}
	}
	else return;
}

// 플레이어가 공격거리 내에 있는지 확인
void Enemy::CheckPlayerInAttackRange()
{
	// 거리의 제곱 계산 (제곱근으로 거리를 계산하는 것보다 빠름)
	if (Vector2D::LengthSquare(playerPosition, m_position) <= attackRange * attackRange)
	{
		if (currentState != EnemyState::ATTACK && currentState != EnemyState::DAMAGED)
			ChangeState(EnemyState::ATTACK);
	}
	else return;
}

// 움직일 방향을 체크하는 함수
void Enemy::CheckMoveDirection()
{
	// 랜덤한 시간마다 실행
	if ((SDL_GetTicks() / 100) % randomWaitTime == 0)
	{
		// 랜덤 값은 위 if문이 실행될 때 한 번만 가져오도록 bool 변수 사용
		if (!moveDirRefreshedFlag)
		{
			moveDirRefreshedFlag = true;
			randomWaitTime = Utility::GetRandomInt(5, 7);
			randomDirection = Utility::GetRandomInt(0, 2);
		}

		// 현재 패트롤 상태에 따라 실행
		switch (currentPatrolState)
		{
		// 패트롤(정찰) 상태라면 무작위 방향 가져오기
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
		// 체이싱(추적) 상태라면 플레이어 위치 쪽으로 방향 정하기
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
		// 랜덤 값 가져오는 bool 플래그 false
		moveDirRefreshedFlag = false;
	}
}

// 공격 액션 함수
void Enemy::Attack()
{
	// 한 번만 실행되어야 하므로 bool 플래그 사용
	attackFlag = true;
	
	// 사운드 재생
	TheAudio::Instance()->PlaySFX("Hit");
	// 공격 방향 및 범위에 대한 처리
	attackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 16 : -16 - attackArea.w);
	attackArea.y = m_position.getY();
	// 실제 공격 부분에 대한 처리
	for (const auto& player : TheGame::Instance()->GetGameObjects())
	{
		if (Collision::onCollision(&attackArea, player))
		{
			if (dynamic_cast<SDLGameObject*>(player)->GetTag() == "Player")
			{
				player->OnHit(damageAmount);
			}
		}
	}
}

// 움직임에 대한 처리
void Enemy::Move()
{
	// 가짜 값을 가져온다
	int fakeYPos = m_position.getY() + m_height + 4;
	int fakeXPos = m_position.getX();

	// 움직임 방향에 따라 변경
	switch (moveDir)
	{
	case moveDirection::RIGHT:
		// 만약 오른쪽 아래에 타일이 있다면 이동하고
		if (TheMap::Instance()->IsTileThere((fakeXPos + m_width + 2) / TILE_SIZE, fakeYPos / TILE_SIZE))
		{
			m_velocity.setX(moveSpeed);
		}
		else // 없다면(낭떠러지라면) 중지하고 방향을 반대로 바꾼다
		{
			m_velocity.setX(0);
			moveDir = moveDirection::LEFT;
		}
		flip = SDL_FLIP_NONE;
		break;
	case moveDirection::LEFT:
		// 만약 왼쪽 아래에 타일이 있다면 이동하고
		if (TheMap::Instance()->IsTileThere((fakeXPos - 2) / TILE_SIZE, fakeYPos / TILE_SIZE))
		{
			m_velocity.setX(-moveSpeed);
		}
		else // 없다면 중지하고 방향을 반대로 바꾼다
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
}

// 넉백 함수
void Enemy::Knockback()
{
	m_velocity.setX((playerPosition.getX() - m_position.getX() > 0 ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
}

// 움직임 방향에 따라 flip 변경
void Enemy::Flipping()
{
	if (moveDir == moveDirection::RIGHT)
	{
		flip = SDL_FLIP_NONE;
	}
	else if (moveDir == moveDirection::LEFT)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
}

// Enemy의 충돌 체크 코드
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