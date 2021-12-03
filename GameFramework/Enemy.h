#pragma once
#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
public:
	Enemy(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void OnHit();

private:
	enum class EnemyState { IDLE, MOVE, ATTACK, DEAD, DAMAGED };
	enum class PatrolState { PATROL, CHASING };
	EnemyState currentState = EnemyState::IDLE;
	PatrolState currentPatrolState = PatrolState::PATROL;

	void ChangeState(EnemyState state);
	void ChangePartrolState(PatrolState state);

	void CheckCollision();
	void CheckPlayerInRange();
	void CheckPlayerInAttackRange();
	void CheckMoveDirection();
	void Attack();
	void Move();
	void Knockback();
	void Flipping();

	void UpdateInState();

	Vector2D playerPosition;

	bool attackFlag = false;
	bool moveDirRefreshedFlag = false;
	bool isGrounded = true;
	enum class moveDirection { RIGHT, LEFT, STOP };
	moveDirection moveDir = moveDirection::STOP;

	int damagedTime = 0;
	int deadTime = 0;
	int attackStartTime = 0;
	int knockbackTime = 200;
	int randomWaitTime = 3;
	int randomDirection = 2;

	int moveSpeed = 2;
	float attackRange = 48;
	float chasingDetectRange = 128+32;
	float knockbackPower = 4;
	SDL_Rect attackArea = { 0,0,32,32 };
};