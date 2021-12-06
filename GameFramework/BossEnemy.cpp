#include "BossEnemy.h"
#include "ScoreManager.h"
#include "Utility.h"
#include "Collision.h"
#include "Game.h"

// 초기화
BossEnemy::BossEnemy(const LoaderParams* pParams) : EnemyBase(pParams)
{
	tag = "Enemy";
	life = 50;
	maxLife = life;
	score = 200;
}

void BossEnemy::draw()
{
	EnemyBase::draw();
}

// 피격당하는 함수
void BossEnemy::OnHit(int amount)
{
	if (currentPattern != PatternState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// 스코어에 점수 추가
			TheScore::Instance()->AddScore(score);
			ChangeState(PatternState::DEAD);
		}

		std::cout << "보스 생명력: " << life << std::endl;
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
			std::cout << "보스 버프 끝!" << std::endl;
		}
	}
}

// 무작위 패턴 고르기 함수
void BossEnemy::ChooseRandomPattern()
{
	int randomInt = Utility::GetRandomInt(0, 4);

	// 패턴의 쿨다운을 검사
	if (SDL_GetTicks() >= patternStartedTime[randomInt] + patternCooldown[randomInt])
	{
		std::cout << "패턴 정상실행: " << randomInt << ", 시간: " << patternCooldown[randomInt] << std::endl;
		ChangeState(static_cast<PatternState>(randomInt));
	}
	else
	{
		actionFlag = false;
		std::cout << "패턴 시도했지만 아직 쿨다운!: " << randomInt << ", 남은 시간: " << patternStartedTime[randomInt] + patternCooldown[randomInt] - SDL_GetTicks() << std::endl;
		return;
	}
}

// 상태 별 다른 업데이트
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

		if (!actionFlag)
		{
			actionFlag = true;
			ChooseRandomPattern();
		}
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
		case 3:
		case 4:
			m_currentFrame = 2;
			break;
		case 5:
			m_currentFrame = 3;
			whirlAttackArea.x = m_position.getX() + 8;
			whirlAttackArea.y = m_position.getY() + 32;
			Attack(&whirlAttackArea);
			break;
		case 6:
		case 7:
			m_currentFrame = 4;
			break;
		case 8:
			m_currentFrame = 5;
			Attack(&whirlAttackArea); // 두 번은 피격당하지 않으나(attackFlag) 공격 판정은 두 번임
			break;
		case 9:
			m_currentFrame = 6;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
			m_currentFrame = 7;
			break;
		case 14:
		default:
			ChangeState(PatternState::IDLE);
			break;
		}
		break;

	case PatternState::DEAD:
		m_currentRow = 7;

		if ((SDL_GetTicks() - deadTime / 150) % 6 < 6)
			m_currentFrame = (SDL_GetTicks() - deadTime / 150) % 6;
		else
			m_currentFrame = 5;
		break;

	case PatternState::ChargeRun:
		m_currentRow = 1;
		m_currentFrame = (SDL_GetTicks() / 70) % 8;

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

		// 실제 공격 부분에 대한 처리
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

// 패턴 상태 변경 함수
void BossEnemy::ChangeState(PatternState state)
{
	switch (currentPattern)
	{
		case PatternState::IDLE:
			Flipping();
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
		std::cout << "보스 패턴: " << static_cast<int>(currentPattern) << ", " << patternStartedTime[static_cast<int>(currentPattern)] << std::endl;
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
		std::cout << "보스 버프 적용됨: 피해량 1 추가" << std::endl;
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
		break;
	default:
		break;
	}
}

void BossEnemy::clean()
{
	EnemyBase::clean();
}