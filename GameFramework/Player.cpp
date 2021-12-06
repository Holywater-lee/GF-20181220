#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "Collision.h"
#include "Camera.h"
#include "FXAnimation.h"
#include "Audio.h"
#include "UIManager.h"
#include "MapManager.h"

#include "WIDTHHEIGHT.h"

const int MOVESPEED = 3;
const int DASHSPEED = 8;
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
	{
		CheckCollisionWithMove();
	}

	TheCam::Instance()->Update(this);

	if (m_position.getY() >= LEVEL_HEIGHT)
	{
		m_position.setX(32 * 2);
		m_position.setY(32 * 16);

		m_acceleration.setY(0);
		m_velocity.setY(0);
	}
}

// �¾��� �� ȣ��Ǵ� �Լ�
void Player::OnHit(int amount)
{
	if (m_currentState != PlayerState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			life = 0;
			ChangeState(PlayerState::DEAD);
		}
		else if (amount > 0)
		{
			ChangeState(PlayerState::DAMAGED);
			TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? 10 : -10, -20);
		}
		
		TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX(), m_position.getY() + 16, 32, 32, "FXHit"), SDL_GetTicks(), 300, 0, 0, false, 7));
		UIManager::Instance()->RefreshHPBar(life);
	}
}

// ������ ���� �� ��ǲ���� �޴� �Լ�
void Player::IdleInput()
{
	if (KeyDown(SDL_SCANCODE_RIGHT) || KeyDown(SDL_SCANCODE_LEFT))
	{
		ChangeState(PlayerState::MOVE);
	}
}

// ������ �� ��ǲ���� �޴� �Լ�
void Player::MoveInput()
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

// ���� �Է°��� �޴� �Լ�
void Player::JumpInput()
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

// ���� ��ǲ���� �޴� �Լ�
void Player::AttackInput()
{
	if (KeyDown(SDL_SCANCODE_A))
	{
		ChangeState(PlayerState::ATTACK);
	}
}

// �뽬 ��ǲ���� �޴� �Լ�
void Player::DashInput()
{
	if (KeyDown(SDL_SCANCODE_S))
	{
		if (SDL_GetTicks() >= dashStartTime + dashDelay)
		{
			ChangeState(PlayerState::DASH);
		}
	}
}

// ���⸦ �����ϴ� ��ǲ���� �޴� �Լ�
void Player::ChangeWeapon()
{
	if (KeyDown(SDL_SCANCODE_DOWN))
	{
		if (SDL_GetTicks() >= nextWeaponChangeDelay)
		{
			cout << "���� ���� �õ�" << endl;
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

// �÷��̾� ���� ����� ��Ʈ��Ƽ�� �������� �����ϴ� �Լ�
void Player::SetAttackStrategy(PlayerAttackStrategy* strategy)
{
	if (attackStrategy != nullptr)
	{
		delete attackStrategy;
		attackStrategy = nullptr;
		std::cout << "���� ��Ʈ��Ƽ�� ������: " << attackStrategy << std::endl;
	}
	attackStrategy = strategy;
	std::cout << "��Ʈ��Ƽ�� ���� �Ϸ�: " << attackStrategy << std::endl;
}

// ���º��� ������Ʈ���� �ٸ��� �����ϴ� �Լ�
void Player::UpdateInState()
{
	switch (m_currentState)
	{
	// ������ �ִ� ����
	case PlayerState::IDLE:
		m_currentRow = 0;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// �����̴� ����
	case PlayerState::MOVE:
		m_currentRow = 3;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// ���� ����
	case PlayerState::JUMP:
		m_currentRow = 4;
		m_currentFrame = ((SDL_GetTicks() / 100) % 4);
		break;
	// ���� ����
	case PlayerState::ATTACK:
		if (isRanged) // ���Ÿ� ���ݸ���� ���
		{
			m_currentRow = 2;

			// switch���� ����� ������ �����Ӹ��� �ٸ� ����ӵ��� �ְ� �;���
			switch ((SDL_GetTicks() - attackStartTime) / 100)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				m_currentFrame = 0;
				break;
			case 4:
				m_currentFrame = 1;
				// ������ �� �ѹ��� �̷������ ������ attackFlag��� bool���� �ε����ϰ� ��� (������� ������ �� ������ ���ݵ�)
				if (!attackFlag)
				{
					attackFlag = true;
					// ��Ʈ��Ƽ���� �ش��ϴ� ����ó��
					AttackActionWithStrategy();
				}
				break;
			case 5:
			case 6:
				m_currentFrame = 2;
				break;
			case 7:
				m_currentFrame = 3;
				break;
			case 8:
			default:
				ChangeState(PlayerState::IDLE);
				break;
			}
		}
		else // �ٰŸ� ���ݸ���� ���
		{
			m_currentRow = 1;

			// switch���� ����� ������ �����Ӹ��� �ٸ� ����ӵ��� �ְ� �;���
			switch ((SDL_GetTicks() - attackStartTime) / 100)
			{
			case 0:
				m_currentFrame = 0;
				break;
			case 1:
			case 2:
				m_currentFrame = 1;
				break;
			case 3:
				m_currentFrame = 2;

				// ������ �� �ѹ��� �̷������ ������ attackFlag��� bool���� �ε����ϰ� ��� (������� ������ �� ������ ���ݵ�)
				if (!attackFlag)
				{
					attackFlag = true;
					// ��Ʈ��Ƽ���� �ش��ϴ� ����ó��
					AttackActionWithStrategy();
				}
				break;
			case 4:
			case 5:
			case 6:
				m_currentFrame = 3;
				break;
			case 7:
			default:
				ChangeState(PlayerState::IDLE);
				break;
			}
		}
		break;
	// ����� ����
	case PlayerState::DEAD:
		// switch���� ����� ������ �����Ӹ��� �ٸ� ����ӵ��� �ְ� �;���
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
	// ���ظ� �Դ� ����
	case PlayerState::DAMAGED:
		m_currentRow = 5;
		m_currentFrame = 0;
		// ���ظ� ���� �ð� + �˹� �ð��� SDL_GetTicks()�� �Ѿ��ٸ� IDLE���·� ����
		if (SDL_GetTicks() >= damagedTime + knockbackTime)
		{
			ChangeState(PlayerState::IDLE);
		}
		break;
	// �뽬 ����
	case PlayerState::DASH:
		m_currentRow = 7;
		m_currentFrame = 0;

		if (SDL_GetTicks() >= dashStartTime + dashDuration)
		{
			ChangeState(PlayerState::IDLE);
		}
		break;
	default:
		break;
	}
}

// �����Ӱ� �浹üũ�� �Բ� �ϴ� �Լ�
void Player::CheckCollisionWithMove()
{
	// ���� �پ����� �ʴٸ� �߷°��� �����ϰ�, ���� �پ��ְ� ���� ���̾��ٸ� IDLE���·� ����
	if (!isGrounded)
	{
		m_acceleration.setY(GRAVITY);
	}
	else
	{
		if (m_currentState == PlayerState::JUMP)
			ChangeState(PlayerState::IDLE);
	}

	// ��ó�� Ÿ�� ������ ������ ����
	std::vector<SDL_Rect> collisionRects;

	// ��ó�� Ÿ���� �ִٸ� ���� collisionRects ���Ϳ� �����Ѵ�
	for (int i = (m_position.getY() - TILE_SIZE) / TILE_SIZE; i <= (m_position.getY() + (TILE_SIZE * 3)) / TILE_SIZE; i++)
	{
		for (int k = (m_position.getX() - TILE_SIZE) / TILE_SIZE; k <= (m_position.getX() + (TILE_SIZE * 2)) / TILE_SIZE; k++)
		{
			if (TheMap::Instance()->IsTileThere(k, i))
			{
				SDL_Rect tempRect = { k * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				collisionRects.emplace_back(std::move(tempRect));
			}
		}
	}

	// x�� �̵�
	m_velocity.setX(m_velocity.getX() + m_acceleration.getX());
	m_position.setX(m_position.getX() + m_velocity.getX());
	// ��ó�� Ÿ�Ͽ� ���ؼ��� �浹 üũ (����ȭ) / x�� �浹 üũ
	for (int i = 0; i < collisionRects.size(); i++)
	{
		if (Collision::onCollision(&collisionRects[i], this))
		{
			if (m_velocity.getX() > 0)
			{
				m_position.setX(collisionRects[i].x - m_width);
			}
			else if (m_velocity.getX())
			{
				m_position.setX(collisionRects[i].x + collisionRects[i].w);
			}
			m_velocity.setX(0);
		}
	}
	// x�� ȭ�� ������ ������ ���ϰ� �Ѵ�
	if (m_position.getX() <= 0)
	{
		m_position.setX(0);
	}
	else if (m_position.getX() + m_width >= LEVEL_WIDTH)
	{
		m_position.setX(LEVEL_WIDTH - m_width);
	}

	// y�� �̵�
	m_velocity.setY(m_velocity.getY() + m_acceleration.getY());
	m_position.setY(m_position.getY() + m_velocity.getY());
	// ��ó�� Ÿ�Ͽ� ���ؼ��� �浹 üũ (����ȭ) / y�� �浹 üũ
	for (int i = 0; i < collisionRects.size(); i++)
	{
		if (Collision::onCollision(&collisionRects[i], this))
		{
			if (m_velocity.getY() > 0)
			{
				isGrounded = true;
				currentJumpCount = 0;
				m_position.setY(collisionRects[i].y - m_height);
			}
			else if (m_velocity.getY() < 0)
			{
				m_position.setY(collisionRects[i].y + collisionRects[i].h);
				m_velocity.setY(0);
			}

			if (m_currentState != PlayerState::JUMP)
			{
				m_acceleration.setY(0);
				m_velocity.setY(0);
			}
		}
	}

	// ���� ��� �ִ����� ���� ó�� (isGrounded)
	m_position.setY(m_position.getY() + 1);
	int count = 0;
	for (auto& tile : collisionRects)
	{
		if (Collision::onCollision(&tile, this))
		{
			count++;
		}
	}
	m_position.setY(m_position.getY() - 1);

	if (count == 0)
	{
		isGrounded = false;
		if (m_currentState != PlayerState::JUMP && m_currentState != PlayerState::DASH)
			ChangeState(PlayerState::JUMP);

		// ���� ������ ���� ���¿��� ���������� �������� ���� Ƚ���� ����
		if (currentJumpCount == 0) currentJumpCount = 1;
	}
}

// ���¸� �������ִ� �Լ�
void Player::ChangeState(PlayerState state)
{
	// ���� ������ �� ó��
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
		break;
	case PlayerState::DASH:
		break;
	default:
		break;
	}

	// ���¸� ����
	m_currentState = state;

	// ������ ���¿� ���� ó��
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
		// �����̶�� �� ������ ����Ʈ ����
		if (!isRanged)
			TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX() + m_width / 2 - 16, m_position.getY() - 30, 32, 64, "FXSword"), SDL_GetTicks(), 350, 0));
		break;
	case PlayerState::DEAD:
		deadTime = SDL_GetTicks();
		TheUI::Instance()->SetGameOverUI(false);
		break;
	case PlayerState::DAMAGED:
		Knockback();
		damagedTime = SDL_GetTicks();
		break;
	case PlayerState::DASH:
		TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(m_position.getX(), m_position.getY(), 32, 60, "PlayerHallucination"), SDL_GetTicks(), 300, m_currentRow, m_currentFrame, flip == SDL_FLIP_HORIZONTAL, 1));
		TheAudio::Instance()->PlaySFX("Hallucination");
		MoveInput();
		m_velocity.setX(DASHSPEED * (flip == SDL_FLIP_NONE ? 1 : -1));
		m_velocity.setY(0);
		dashStartTime = SDL_GetTicks();
		break;
	default:
		break;
	}
}

// �˹� �׼� �Լ�
void Player::Knockback()
{
	m_velocity.setX((flip == SDL_FLIP_NONE ? -1 : 1) * knockbackPower);
	m_velocity.setY(-knockbackPower / 2);
}

// Ű���� �Է��� �����ϴ� �Լ�
bool Player::KeyDown(SDL_Scancode code)
{
	return TheInputHandler::Instance()->isKeyDown(code);
}

void Player::handleInput()
{
	// ������ ���¸��� ���� �� �ִ� ��ư ����
	switch (m_currentState)
	{
	case PlayerState::IDLE:
		IdleInput();
		JumpInput();
		AttackInput();
		DashInput();
		ChangeWeapon();
		break;

	case PlayerState::MOVE:
		MoveInput();
		JumpInput();
		AttackInput();
		DashInput();
		ChangeWeapon();
		break;

	case PlayerState::JUMP:
		MoveInput();
		JumpInput();
		DashInput();
		ChangeWeapon();
		break;

	case PlayerState::ATTACK:
		DashInput();
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