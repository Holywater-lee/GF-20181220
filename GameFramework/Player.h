#pragma once
#include "SDLGameObject.h"
#include "PlayerAttackStrategy.h"

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
	~Player() { delete attackStrategy; }
	virtual void draw();
	virtual void update();
	virtual void clean();

private:
	enum class PlayerState { IDLE = 0, MOVE, JUMP, ATTACK };
	void handleInput();
	void UpdateFrame();
	bool KeyDown(SDL_Scancode code);

	void CheckCollision();
	void ChangeState(PlayerState state);

	void Idle();
	void Move();
	void Jump();
	void Attack();
	void ChangeWeapon();
	void SetAttackStrategy(PlayerAttackStrategy* strategy);

	void AttackActionWithStrategy()
	{
		attackStrategy->AttackAction(this, &attackArea, flip);
	}

	PlayerState m_currentState = PlayerState::IDLE;
	PlayerAttackStrategy* attackStrategy = new RangedAttackStrategy();

	int multipleJumpDelay = 300;
	int nextJumpDelay = 0;

	int currentJumpCount = 0;
	int maxJumpCount = 2;

	int attackDelay = 500;
	int nextAttackDelay = 0;
	int attackStartTime = 0;

	int weaponChangeDelay = 3000;
	int nextWeaponChangeDelay = 0;

	bool isGrounded = false;
	bool attackFXFlag = false;
	bool isRanged = true;
	SDL_Rect attackArea = { 0,0,32,64 };
};