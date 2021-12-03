#pragma once
#include "SDLGameObject.h"
#include "PlayerAttackStrategy.h"

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
	~Player() { delete attackStrategy; attackStrategy = nullptr; }
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void OnHit();

	// 플레이어가 사망했는지 가져오는 함수
	bool GetIsDead() { return (m_currentState == PlayerState::DEAD); }

private:
	enum class PlayerState { IDLE = 0, MOVE, JUMP, ATTACK, DEAD, DAMAGED };
	void handleInput();
	void UpdateInState();
	bool KeyDown(SDL_Scancode code);

	void CheckCollisionWithMove();
	void ChangeState(PlayerState state);

	void Idle();
	void Move();
	void Jump();
	void Attack();
	void Knockback();
	void ChangeWeapon();
	void SetAttackStrategy(PlayerAttackStrategy* strategy);

	// 현재 스트래티지에 따른 공격처리
	void AttackActionWithStrategy()
	{
		attackStrategy->AttackAction(this, &attackArea, flip);
	}

	// 플레이어 현재 상태
	PlayerState m_currentState = PlayerState::IDLE;
	// 플레이어 현재 공격 스트래티지
	PlayerAttackStrategy* attackStrategy = new RangedAttackStrategy();

	int currentJumpCount = 0;
	int maxJumpCount = 2;

	int knockbackPower = 25;

	// 시간에 관한 변수들
	int multipleJumpDelay = 300;
	int nextJumpDelay = 0;
	int attackDelay = 500;
	int nextAttackDelay = 0;
	int attackStartTime = 0;
	int damagedTime = 0;
	int knockbackTime = 200;
	int deadTime = 0;
	int weaponChangeDelay = 2000;
	int nextWeaponChangeDelay = 0;

	bool isGrounded = false;
	bool attackFlag = false;
	bool isRanged = true;
	// 공격 범위
	SDL_Rect attackArea = { 0,0,32,64 };
};