#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "Collision.h"
#include "Camera.h"
#include "FXAnimation.h"
#include "Audio.h"
#include "UIManager.h"

#include "WIDTHHEIGHT.h"

const int MOVESPEED = 3;
const int JUMPHEIGHT = 8;

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Player";
	life = 5;
	maxLife = life;
}

void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update()
{
	if (m_currentState == PlayerState::DEAD)
	{
		if (KeyDown(SDL_SCANCODE_R))
		{
			TheGame::Instance()->RestartGame();
			return;
		}
	}

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
}

// 맞았을 때 호출되는 함수
void Player::OnHit(int amount)
{
	if (m_currentState != PlayerState::DEAD)
	{
		life -= amount;
		std::cout << "피격 " << life << ", " << maxLife << std::endl;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			life = 0;
			ChangeState(PlayerState::DEAD);
			std::cout << "사망!" << std::endl;
		}
		else if(amount > 0)
		{
			ChangeState(PlayerState::DAMAGED);
			std::cout << "타격 당함! 남은 체력: " << life << std::endl;
		}

		UIManager::Instance()->RefreshHPBar(life);
	}
}

// 가만히 있을 때 인풋값을 받는 함수
void Player::Idle()
{
	if (KeyDown(SDL_SCANCODE_RIGHT) || KeyDown(SDL_SCANCODE_LEFT))
	{
		ChangeState(PlayerState::MOVE);
	}
}

// 움직일 때 인풋값을 받는 함수
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

// 점프 입력값을 받는 함수
void Player::Jump()
{
	if (KeyDown(SDL_SCANCODE_UP))
	{
		if (SDL_GetTicks() >= nextJumpDelay && currentJumpCount < maxJumpCount)
		{
			nextJumpDelay = SDL_GetTicks() + multipleJumpDelay;

			m_velocity.setY(-JUMPHEIGHT);
			currentJumpCount++;
			TheAudio::Instance()->PlaySFX("Jump");
		}

		if (m_currentState != PlayerState::JUMP && !isGrounded)
			ChangeState(PlayerState::JUMP);
	}
}

// 공격 인풋값을 받는 함수
void Player::Attack()
{
	if (KeyDown(SDL_SCANCODE_A))
	{
		ChangeState(PlayerState::ATTACK);
	}
}

// 무기를 변경하는 인풋값을 받는 함수
void Player::ChangeWeapon()
{
	if (KeyDown(SDL_SCANCODE_DOWN))
	{
		if (SDL_GetTicks() >= nextWeaponChangeDelay)
		{
			cout << "무기 변경 시도" << endl;
			if (isRanged)
			{
				SetAttackStrategy(new MeleeAttackStrategy());
				UIManager::Instance()->SetWeaponIcon("IconSword");
			}
			else
			{
				SetAttackStrategy(new RangedAttackStrategy());
				UIManager::Instance()->SetWeaponIcon("IconGun");
			}

			nextWeaponChangeDelay = SDL_GetTicks() + weaponChangeDelay;
			isRanged = !isRanged;
		}
		else
		{
			UIManager::Instance()->ShakeIcon();
		}
	}
}

// 플레이어 공격 방식을 스트래티지 패턴으로 변경하는 함수
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

// 상태별로 업데이트문을 다르게 실행하는 함수
void Player::UpdateInState()
{
	switch (m_currentState)
	{
	// 가만히 있는 상태
	case PlayerState::IDLE:
		m_currentRow = 0;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// 움직이는 상태
	case PlayerState::MOVE:
		m_currentRow = 3;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// 점프 상태
	case PlayerState::JUMP:
		m_currentRow = 4;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// 공격 상태
	case PlayerState::ATTACK:
		if (isRanged) // 원거리 공격모드일 경우
		{
			m_currentRow = 2;

			// switch문을 사용한 이유는 프레임마다 다른 재생속도를 주고 싶었음
			switch ((SDL_GetTicks() - attackStartTime) / 100)
			{
			case 0:
			case 1:
			case 2:
				m_currentFrame = 0;
				break;
			case 3:
				m_currentFrame = 1;
				// 공격은 단 한번만 이루어져야 함으로 attackFlag라는 bool값을 부득이하게 사용 (사용하지 않으면 매 프레임 공격됨)
				if (!attackFlag)
				{
					attackFlag = true;
					// 스트래티지에 해당하는 공격처리
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
		else // 근거리 공격모드일 경우
		{
			m_currentRow = 1;

			// switch문을 사용한 이유는 프레임마다 다른 재생속도를 주고 싶었음
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

				// 공격은 단 한번만 이루어져야 함으로 attackFlag라는 bool값을 부득이하게 사용 (사용하지 않으면 매 프레임 공격됨)
				if (!attackFlag)
				{
					attackFlag = true;
					// 스트래티지에 해당하는 공격처리
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
	// 사망한 상태
	case PlayerState::DEAD:
		// switch문을 사용한 이유는 프레임마다 다른 재생속도를 주고 싶었음
		switch ((SDL_GetTicks() - deadTime) / 100)
		{
		case 0:
			m_currentRow = 5;
			m_currentFrame = 0;
			break;
		case 1:
		case 2:
			m_currentFrame = 1;
			break;
		case 3:
		case 4:
		case 5:
			m_currentFrame = 2;
			break;
		case 6:
		case 7:
			m_currentFrame = 3;
			break;
		case 8:
			m_currentRow = 6;
			m_currentFrame = 0;
			break;
		case 9:
		case 10:
			m_currentFrame = 1;
			break;
		case 11:
		case 12:
			m_currentFrame = 2;
			break;
		default:
			m_currentFrame = 3;
			break;
		}
		break;
	// 피해를 입는 상태
	case PlayerState::DAMAGED:
		m_currentRow = 5;
		m_currentFrame = 0;
		// 피해를 입은 시간 + 넉백 시간을 SDL_GetTicks()가 넘었다면 IDLE상태로 변경
		if (SDL_GetTicks() >= damagedTime + knockbackTime)
		{
			ChangeState(PlayerState::IDLE);
		}
		break;
	default:
		break;
	}
}

// 움직임과 충돌체크를 함께 하는 함수
void Player::CheckCollisionWithMove()
{
	// 땅에 붙어있지 않다면 중력값을 적용하고, 땅에 붙어있고 점프 중이었다면 IDLE상태로 변경
	if (!isGrounded)
	{
		m_acceleration.setY(GRAVITY);
	}
	else
	{
		if (m_currentState == PlayerState::JUMP)
			ChangeState(PlayerState::IDLE);
	}

	// SDLGameObject::update(); 를 사용하지 않고 x축 충돌처리와 y축 충돌처리를 따로 구분하기 위해서 이렇게 구현하였습니다
	// SDLGameObject::update(); 를 사용한 후 각각의 축에 대해 충돌처리 시 대각선 방향에서 충돌하는 경우 원하는 움직임이 구현되지 않았습니다.
	// x축에 대한 처리
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
	// 화면 밖으로 나갈 경우 (x축)
	if (m_position.getX() <= 0)
	{
		m_position.setX(0);
	}
	else if (m_position.getX() + m_width >= LEVEL_WIDTH)
	{
		m_position.setX(LEVEL_WIDTH - m_width);
	}

	// y축에 대한 처리
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
	
	// 땅을 밟고 있는지에 대한 처리 (isGrounded)
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
		if (m_currentState != PlayerState::JUMP)
			ChangeState(PlayerState::JUMP);

		// 점프 중이지 않은 상태에서 낭떠러지로 떨어지면 점프 횟수를 제한
		if (currentJumpCount == 0) currentJumpCount = 1;
	}
}

// 상태를 변경해주는 함수
void Player::ChangeState(PlayerState state)
{
	// 상태 나갔을 때 처리
	switch (m_currentState)
	{
	case PlayerState::IDLE:
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::JUMP:
		break;
	case PlayerState::ATTACK:
		attackFlag = false;
		break;
	case PlayerState::DAMAGED:
		m_velocity.setX(0);
		m_velocity.setY(0);
	default:
		break;
	}

	// 상태를 변경
	m_currentState = state;

	// 진입한 상태에 대한 처리
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
		// 근접이라면 기 모으는 이펙트 생성
		if (!isRanged)
			TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX() + m_width / 2 - 16, m_position.getY() - 30, 32, 64, "FXSword"), SDL_GetTicks(), 350, 0));
		break;
	case PlayerState::DEAD:
		deadTime = SDL_GetTicks();
		break;
	case PlayerState::DAMAGED:
		Knockback();
		damagedTime = SDL_GetTicks();
	default:
		break;
	}
}

// 넉백 액션 함수
void Player::Knockback()
{
	m_velocity.setX((flip == SDL_FLIP_NONE ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
}

// 키보드 입력을 리턴하는 함수
bool Player::KeyDown(SDL_Scancode code)
{
	return TheInputHandler::Instance()->isKeyDown(code);
}

void Player::handleInput()
{
	// 각각의 상태마다 누를 수 있는 버튼 제한
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
	TheCam::Instance()->Clean();
	SDLGameObject::clean();
}