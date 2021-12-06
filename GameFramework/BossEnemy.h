#pragma once
#include "SDLGameObject.h"

class BossEnemy : public SDLGameObject
{
public:
	BossEnemy(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void OnHit(int amount);

private:
	enum class PatternState { IDLE, PT_Charge, PT_ChargeReady, PT_Slash, PT_Stab, PT_Buff, PT_Whirl, DEAD };
	PatternState currentPattern = PatternState::IDLE;

	bool isFoundPlayer = false;
	const int chargeSpeed = 5;
	const int damage = 1;
	const int buffAddDamage = 1;

	int applyDamage;

	//void ChangeState(PatternState state);
};