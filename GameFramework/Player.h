#pragma once
#include "SDLGameObject.h"
#include <vector>

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
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
	void Shoot();
	void Attack();
	
	PlayerState m_currentState = PlayerState::IDLE;

	int ballShootDelay = 500;
	int nextShootDelay = 0;

	int multipleJumpDelay = 300;
	int nextJumpDelay = 0;

	int currentJumpCount = 0;
	int maxJumpCount = 2;

	int attackDelay = 500;
	int nextAttackDelay = 0;
	int attackStartTime = 0;

	bool isGrounded = false;
	bool isAttack = false;
	bool slashFXFlag = false;

	SDL_Rect attackArea = { 0,0,32,64 };
};