#include "Enemy.h"
#include "ScoreManager.h"
#include "Utility.h"
#include "Game.h"
#include "Collision.h"
#include "MapManager.h"
#include "Audio.h"

#include "WIDTHHEIGHT.h"

// �ʱ�ȭ
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

// �ǰݴ��ϴ� �Լ�
void Enemy::OnHit(int amount)
{
	if (currentState != EnemyState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// ���ھ 10�� �߰�
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
	// ��� ���°� �ƴϸ�
	if (currentState != EnemyState::DEAD)
	{
		CheckPlayerInRange(); // �÷��̾ �Ÿ� ���� �ִ��� (Patrol, Chasing)
		CheckPlayerInAttackRange(); // ���� �Ÿ� ���� �ִ��� (��� �Ѿư�����, �����Ұ���)
		CheckMoveDirection(); // ���� �ð����� ���� üũ (������ ������ �ٸ� Enemy�� ��ġ�� �ʰԲ� + ������?)
	}

	UpdateInState();
	SDLGameObject::update();
	CheckCollision();

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		this->clean();
	}
}

// ���º� �ٸ� update
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

// ���¸� �������ִ� �Լ�
void Enemy::ChangeState(EnemyState state)
{
	// ���¿��� ���� �� ó��
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

	// ���� ����
	currentState = state;

	// ����� ���¿� ���� ó��
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

// ��Ʈ�� ���¸� �����ϴ� �Լ�
void Enemy::ChangePatrolState(PatrolState state)
{
	// ��Ʈ�� ���� ����
	currentPatrolState = state;

	// ����� ��Ʈ�� ���¿� ���� ó��
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

// �÷��̾ �����Ÿ� ���� �ִ��� Ȯ���ϴ� �Լ�
void Enemy::CheckPlayerInRange()
{
	// ���� �ֱ⸶�� ����
	if ((SDL_GetTicks() / 100) % 5 == 0)
	{
		// �÷��̾� ��ġ���� �޾ƿ�
		playerPosition = TheGame::Instance()->GetPlayerPos();
		//playerPosition = TheGame::Instance()->FindObjectPositionWithTag("Player");

		// �Ÿ��� ���� ��� (���������� �Ÿ��� ����ϴ� �ͺ��� ����), LengthSquare�� ���� �߰��� Vector2D �Լ�
		if (Vector2D::LengthSquare(playerPosition, m_position) <= chasingDetectRange * chasingDetectRange)
		{
			ChangePatrolState(PatrolState::CHASING);
		}
		else
		{
			ChangePatrolState(PatrolState::PATROL);
		}

		// �������� �ƴ϶�� ������ ���⿡ ���� flip ����
		if (currentState != EnemyState::ATTACK)
		{
			Flipping();
		}
	}
	else return;
}

// �÷��̾ ���ݰŸ� ���� �ִ��� Ȯ��
void Enemy::CheckPlayerInAttackRange()
{
	// �Ÿ��� ���� ��� (���������� �Ÿ��� ����ϴ� �ͺ��� ����)
	if (Vector2D::LengthSquare(playerPosition, m_position) <= attackRange * attackRange)
	{
		if (currentState != EnemyState::ATTACK && currentState != EnemyState::DAMAGED)
			ChangeState(EnemyState::ATTACK);
	}
	else return;
}

// ������ ������ üũ�ϴ� �Լ�
void Enemy::CheckMoveDirection()
{
	// ������ �ð����� ����
	if ((SDL_GetTicks() / 100) % randomWaitTime == 0)
	{
		// ���� ���� �� if���� ����� �� �� ���� ���������� bool ���� ���
		if (!moveDirRefreshedFlag)
		{
			moveDirRefreshedFlag = true;
			randomWaitTime = Utility::GetRandomInt(5, 7);
			randomDirection = Utility::GetRandomInt(0, 2);
		}

		// ���� ��Ʈ�� ���¿� ���� ����
		switch (currentPatrolState)
		{
		// ��Ʈ��(����) ���¶�� ������ ���� ��������
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
		// ü�̽�(����) ���¶�� �÷��̾� ��ġ ������ ���� ���ϱ�
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
		// ���� �� �������� bool �÷��� false
		moveDirRefreshedFlag = false;
	}
}

// ���� �׼� �Լ�
void Enemy::Attack()
{
	// �� ���� ����Ǿ�� �ϹǷ� bool �÷��� ���
	attackFlag = true;
	
	// ���� ���
	TheAudio::Instance()->PlaySFX("Hit");
	// ���� ���� �� ������ ���� ó��
	attackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 16 : -16 - attackArea.w);
	attackArea.y = m_position.getY();
	// ���� ���� �κп� ���� ó��
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

// �����ӿ� ���� ó��
void Enemy::Move()
{
	// ��¥ ���� �����´�
	int fakeYPos = m_position.getY() + m_height + 4;
	int fakeXPos = m_position.getX();

	// ������ ���⿡ ���� ����
	switch (moveDir)
	{
	case moveDirection::RIGHT:
		// ���� ������ �Ʒ��� Ÿ���� �ִٸ� �̵��ϰ�
		if (TheMap::Instance()->IsTileThere((fakeXPos + m_width + 2) / TILE_SIZE, fakeYPos / TILE_SIZE))
		{
			m_velocity.setX(moveSpeed);
		}
		else // ���ٸ�(�����������) �����ϰ� ������ �ݴ�� �ٲ۴�
		{
			m_velocity.setX(0);
			moveDir = moveDirection::LEFT;
		}
		flip = SDL_FLIP_NONE;
		break;
	case moveDirection::LEFT:
		// ���� ���� �Ʒ��� Ÿ���� �ִٸ� �̵��ϰ�
		if (TheMap::Instance()->IsTileThere((fakeXPos - 2) / TILE_SIZE, fakeYPos / TILE_SIZE))
		{
			m_velocity.setX(-moveSpeed);
		}
		else // ���ٸ� �����ϰ� ������ �ݴ�� �ٲ۴�
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

// �˹� �Լ�
void Enemy::Knockback()
{
	m_velocity.setX((playerPosition.getX() - m_position.getX() > 0 ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
}

// ������ ���⿡ ���� flip ����
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

// Enemy�� �浹 üũ �ڵ�
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