#pragma once
#include "SDLGameObject.h"
#include <vector>

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
	virtual void draw();
	virtual void update(float deltaTime);
	virtual void clean();

private:
	enum class PlayerState { IDLE = 0, MOVE, JUMP };
	void handleInput();
	void UpdateFrame();
	bool KeyDown(SDL_Scancode code);

	void CheckCollision(float deltaTime);
	void ChangeState(PlayerState state);

	void Idle();
	void Move();
	void Jump();
	void Shoot();

	PlayerState m_currentState = PlayerState::IDLE;

	int ballShootDelay = 500;
	int nextShootDelay;

	int multipleJumpDelay = 200;
	int nextJumpDelay;

	int currentJumpCount = 0;
	int maxJumpCount = 2;

	bool isGrounded = false;
};