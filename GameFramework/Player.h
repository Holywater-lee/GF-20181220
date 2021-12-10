#pragma once
#include "SDLGameObject.h"
#include "PlayerAttackStrategy.h"

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
	~Player() { delete attackStrategy; attackStrategy = nullptr; }
	virtual void update();
	virtual void clean();
	virtual void OnHit(int amount);

	// �÷��̾ ����ߴ��� �������� �Լ�
	bool GetIsDead() { return (m_currentState == PlayerState::DEAD); }

private:
	enum class PlayerState { IDLE = 0, MOVE, JUMP, ATTACK, DEAD, DAMAGED, DASH };
	void handleInput();
	void UpdateInState();
	bool KeyDown(SDL_Scancode code);

	void CheckCollisionWithMove();
	void ChangeState(PlayerState state);

	void IdleInput();
	void MoveInput();
	void JumpInput();
	void AttackInput();
	void DashInput();
	void Knockback();
	void ChangeWeapon();
	void SetAttackStrategy(PlayerAttackStrategy* strategy);

	// ���� ��Ʈ��Ƽ���� ���� ����ó��
	void AttackActionWithStrategy()
	{
		attackStrategy->AttackAction(this, &attackArea, flip);
	}

	// �÷��̾� ���� ����
	PlayerState m_currentState = PlayerState::IDLE;
	// �÷��̾� ���� ���� ��Ʈ��Ƽ��
	PlayerAttackStrategy* attackStrategy = new RangedAttackStrategy();

	int currentJumpCount = 0;
	const int maxJumpCount = 2;
	const int multipleJumpDelay = 300;
	const int dashDelay = 750;
	const int dashDuration = 200;
	const int knockbackPower = 25;
	const int knockbackTime = 300;
	const int weaponChangeDelay = 2000;

	int nextJumpDelay = 0;
	int dashStartTime = 0;
	int attackStartTime = 0;
	int damagedTime = 0;
	int deadTime = 0;
	int nextWeaponChangeDelay = 0;

	bool isGrounded = false;
	bool attackFlag = false;
	bool isRanged = true;
	// ���� ����
	SDL_Rect attackArea = { 0,0,32,64 };
};