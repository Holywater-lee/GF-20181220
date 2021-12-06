#pragma once
#include "EnemyBase.h"

class BossEnemy : public EnemyBase
{
public:
	BossEnemy(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void OnHit(int amount);

private:
	enum class PatternState { PT_Charge, PT_Slash, PT_Stab, PT_Buff, PT_Whirl, IDLE, DEAD, ChargeRun };
	PatternState currentPattern = PatternState::IDLE;

	void ChangeState(PatternState state);
	void ChooseRandomPattern();
	void UpdateInState();
	void Attack(SDL_Rect* area);
	void Flipping();
	
	int patternStartedTime[5] = {0};
	int chargeStartTime = 0;
	int deadTime = 0;
	int idleEnterTime = 0;
	int buffStartTime = 0;

	bool isFoundPlayer = false;
	bool isBuffed = false;
	bool actionFlag = false;
	bool attackFlag = false;

	const int patternCooldown[5] = {3000, 2000, 1000, 10000, 3000};
	const int chargeSpeed = 5;
	const int chargeDuration = 500;
	const int buffDuration = 4000;
	const int damageAmount = 1;
	const int buffAddDamage = 1;

	SDL_Rect slashAttackArea = { 0, 0, 64, 64 };
	SDL_Rect stabAttackArea = { 0, 0, 64, 32 };
	SDL_Rect whirlAttackArea = { 0, 0, 80, 32 };
};