#include "BossEnemy.h"
#include "ScoreManager.h"
#include "Utility.h"
#include "Collision.h"
#include "Game.h"
#include "Audio.h"
#include "FXAnimation.h"
#include "UIManager.h"

// �ʱ�ȭ
BossEnemy::BossEnemy(const LoaderParams* pParams) : EnemyBase(pParams)
{
	tag = "Enemy";
	life = 40;
	maxLife = life;
	score = 200;
}

void BossEnemy::draw()
{
	EnemyBase::draw();
}

// �ǰݴ��ϴ� �Լ�
void BossEnemy::OnHit(int amount)
{
	if (currentPattern != PatternState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// ���ھ ���� �߰�
			TheScore::Instance()->AddScore(score);
			ChangeState(PatternState::DEAD);
		}

		std::cout << "���� �����: " << life << std::endl;
		TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX() + 32, m_position.getY() + 16, 32, 32, "FXHit"), SDL_GetTicks(), 300, 0, 0, false, 7));
	}
}

void BossEnemy::update()
{
	EnemyBase::update();
	EnemyBase::CheckCollision(4, 3);
	UpdateInState();

	if (SDL_GetTicks() >= buffStartTime + buffDuration)
	{
		if (isBuffed)
		{
			isBuffed = false;
			std::cout << "���� ���� ��!" << std::endl;
		}
	}
}

// ������ ���� ���� �Լ�
void BossEnemy::ChooseRandomPattern()
{
	int randomInt = Utility::GetRandomInt(0, 4);

	// ������ ��ٿ��� �˻�
	if (SDL_GetTicks() >= patternStartedTime[randomInt] + patternCooldown[randomInt])
	{
		std::cout << "���� �������: " << randomInt << ", �ð�: " << patternCooldown[randomInt] << std::endl;
		ChangeState(static_cast<PatternState>(randomInt));
	}
	else
	{
		actionFlag = false;
		std::cout << "���� �õ������� ���� ��ٿ�!: " << randomInt << ", ���� �ð�: " << patternStartedTime[randomInt] + patternCooldown[randomInt] - SDL_GetTicks() << std::endl;
		return;
	}
}

void BossEnemy::CheckPlayer()
{
	Vector2D playerPos = TheGame::Instance()->GetPlayerPos();

	if (!isFoundPlayer)
	{
		// �÷��̾ ���� Ư�� ��ġ�� �ִٸ�
		if (playerPos.getX() / TILE_SIZE > 50 && playerPos.getX() / TILE_SIZE < 58 && playerPos.getY() / TILE_SIZE < 19)
		{
			isFoundPlayer = true;
			TheAudio::Instance()->PlayBGM("Boss");
		}
	}
	else
	{
		if (Vector2D::LengthSquare(playerPos, m_position) <= attackDetectRange * attackDetectRange)
		{
			isPlayerInAttackRange = true;

			if (!attackFlag)
			{
				actionFlag = true;
				ChooseRandomPattern();
			}
			else if (currentPattern == PatternState::MOVE)
				ChangeState(PatternState::IDLE);
		}
		else
		{
			isPlayerInAttackRange = false;

			if (currentPattern != PatternState::MOVE)
				ChangeState(PatternState::MOVE);
		}
	}
}

// ���� �� �ٸ� ������Ʈ
void BossEnemy::UpdateInState()
{
	switch (currentPattern)
	{
	case PatternState::IDLE:
		m_currentRow = 0;

		m_currentFrame = (SDL_GetTicks() / 100) % 5;

		if (SDL_GetTicks() >= idleEnterTime + 500)
		{
			actionFlag = false;
		}

		CheckPlayer();
		break;

	case PatternState::MOVE:
		m_currentRow = 1;

		m_currentFrame = (SDL_GetTicks() / 100) % 8;
		Flipping();
		m_velocity.setX(moveSpeed * (flip == SDL_FLIP_NONE ? 1 : -1));
		CheckPlayer();
		break;

	case PatternState::PT_Charge:
		m_currentRow = 2;

		switch ((SDL_GetTicks() - patternStartedTime[0]) / 100)
		{
			case 0:case 1:case 2:case 3:case 4:
			case 5:case 6:case 7:case 8:case 9:
				m_currentFrame = ((SDL_GetTicks() - patternStartedTime[0]) / 100) % 5;
				break;
			case 10:
			default:
				ChangeState(PatternState::ChargeRun);
				break;
		}
		break;

	case PatternState::PT_Slash:
		m_currentRow = 3;

		switch ((SDL_GetTicks() - patternStartedTime[1]) / 100)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			m_currentFrame = 0;
				break;
		case 4:
			m_currentFrame = 1;
			slashAttackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 0 : -slashAttackArea.w);
			slashAttackArea.y = m_position.getY();
			Attack(&slashAttackArea);
			break;
		case 5:
		case 6:
			m_currentFrame = 2;
			break;
		case 7:
			m_currentFrame = 3;
			break;
		case 8:
		case 9:
			m_currentFrame = 4;
			break;
		case 10:
			m_currentFrame = 6;
			break;
		case 11:
		default:
			ChangeState(PatternState::IDLE);
			break;
		}
		break;

	case PatternState::PT_Stab:
		m_currentRow = 4;

		switch ((SDL_GetTicks() - patternStartedTime[2]) / 100)
		{
		case 0:
		case 1:
			m_currentFrame = 0;
			break;
		case 2:
		case 3:
			m_currentFrame = 1;
			break;
		case 4:
			m_currentFrame = 2;
			stabAttackArea.x = m_position.getX() + m_width / 2 + (flip == SDL_FLIP_NONE ? 0 : -stabAttackArea.w);
			stabAttackArea.y = m_position.getY() + 32;
			Attack(&stabAttackArea);
			break;
		case 5:
		case 6:
			m_currentFrame = 3;
			break;
		case 7:
		case 8:
			m_currentFrame = 4;
			break;
		case 9:
			m_currentFrame = 5;
			break;
		case 10:
		default:
			ChangeState(PatternState::IDLE);
			break;
		}
		break;

	case PatternState::PT_Buff:
		m_currentRow = 5;

		switch ((SDL_GetTicks() - patternStartedTime[3]) / 100)
		{
		case 0:
		case 1:
		case 2:
			m_currentFrame = 0;
			break;
		case 3:
			m_currentFrame = 1;
			break;
		case 4:
			m_currentFrame = 2;
			break;
		case 5:
		case 6:
		case 7:
			m_currentFrame = 3;
			break;
		case 8:
			m_currentFrame = 4;
			break;
		case 9:
			m_currentFrame = 5;
			break;
		case 10:
		default:
			ChangeState(PatternState::IDLE);
			break;
		}
		break;

	case PatternState::PT_Whirl:
		m_currentRow = 6;

		switch ((SDL_GetTicks() - patternStartedTime[4]) / 100)
		{
		case 0:
			m_currentFrame = 0;
			break;
		case 1:
			m_currentFrame = 1;
			break;
		case 2:
			m_currentFrame = 2;
			break;
		case 3:
			m_currentFrame = 3;
			whirlAttackArea.x = m_position.getX();
			whirlAttackArea.y = m_position.getY() + 32;
			Attack(&whirlAttackArea);
			break;
		case 4:
		case 5:
			m_currentFrame = 4;
			break;
		case 6:
			m_currentFrame = 5;
			Attack(&whirlAttackArea); // �� ���� �ǰݴ����� ������(attackFlag) ���� ������ �� ����
			break;
		case 7:
			m_currentFrame = 6;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			m_currentFrame = 7;
			break;
		case 12:
		default:
			ChangeState(PatternState::IDLE);
			break;
		}
		break;

	case PatternState::DEAD:
		m_currentRow = 7;

		switch ((SDL_GetTicks() - deadTime) / 100)
		{
		case 0:
			m_currentFrame = 0;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			m_currentFrame = 1;
			break;
		case 5:
		case 6:
			m_currentFrame = 2;
			break;
		case 7:
		case 8:
			m_currentFrame = 3;
			break;
		case 9:
			m_currentFrame = 4;
			break;
		case 10:
		default:
			m_currentFrame = 5;
			break;
		}
		break;

	case PatternState::ChargeRun:
		m_currentRow = 1;
		m_currentFrame = (SDL_GetTicks() / 60) % 8;

		if (SDL_GetTicks() >= chargeStartTime + chargeDuration)
		{
			ChangeState(PatternState::PT_Whirl);
		}
		break;

	default:
		break;
	}
}

void BossEnemy::Attack(SDL_Rect* area)
{
	if (!attackFlag)
	{
		attackFlag = true;

		// ���� ���� �κп� ���� ó��
		for (const auto& player : TheGame::Instance()->GetGameObjects())
		{
			if (Collision::onCollision(area, player))
			{
				if (dynamic_cast<SDLGameObject*>(player)->GetTag() == "Player")
				{
					player->OnHit(damageAmount + (isBuffed ? buffAddDamage : 0));
				}
			}
		}
	}
}

void BossEnemy::Flipping()
{
	if (m_position.getX() + m_width / 2 < TheGame::Instance()->GetPlayerPos().getX() + 16)
	{
		flip = SDL_FLIP_NONE;
	}
	else
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
}

// ���� ���� ���� �Լ�
void BossEnemy::ChangeState(PatternState state)
{
	switch (currentPattern)
	{
		case PatternState::IDLE:
			Flipping();
			break;
		case PatternState::MOVE:
			m_velocity.setX(0);
			break;
		case PatternState::PT_Charge:
			break;
		case PatternState::PT_Slash:
			break;
		case PatternState::PT_Stab:
			break;
		case PatternState::PT_Buff:
			break;
		case PatternState::PT_Whirl:
			break;
		case PatternState::ChargeRun:
			m_velocity.setX(0);
			break;
		case PatternState::DEAD:
			break;
		default:
			break;
	}

	currentPattern = state;

	if (static_cast<int>(currentPattern) < 5)
	{
		patternStartedTime[static_cast<int>(currentPattern)] = SDL_GetTicks();
		std::cout << "���� ����: " << static_cast<int>(currentPattern) << ", " << patternStartedTime[static_cast<int>(currentPattern)] << std::endl;
	}
	
	switch (state)
	{
	case PatternState::IDLE:
		attackFlag = false;
		idleEnterTime = SDL_GetTicks();
		break;
	case PatternState::PT_Charge:
		break;
	case PatternState::PT_Slash:
		break;
	case PatternState::PT_Stab:
		break;
	case PatternState::PT_Buff:
		buffStartTime = SDL_GetTicks();
		isBuffed = true;
		TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX() + 16, m_position.getY(), 64, 64, "FXGathering"), SDL_GetTicks(), 1200, 0, 0, false, 33));
		std::cout << "���� ���� �����: ���ط� 1 �߰�" << std::endl;
		break;
	case PatternState::PT_Whirl:
		break;
	case PatternState::ChargeRun:
		chargeStartTime = SDL_GetTicks();
		m_velocity.setX(chargeSpeed * (flip == SDL_FLIP_NONE ? 1 : -1));
		break;
	case PatternState::DEAD:
		deadTime = SDL_GetTicks();
		m_velocity.setX(0);
		TheUI::Instance()->SetGameOverUI(true);
		break;
	default:
		break;
	}
}

void BossEnemy::clean()
{
	EnemyBase::clean();
}