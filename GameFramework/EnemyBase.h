#pragma once
#include "SDLGameObject.h"

// Enemy�� BossEnemy�� �θ� �Ǵ� Ŭ����
class EnemyBase : public SDLGameObject
{
public:
	EnemyBase(const LoaderParams* pParams) : SDLGameObject(pParams) {}

	virtual ~EnemyBase() {}

protected:
	void CheckCollision(int checkX, int checkY);
	bool isGrounded = true;
	int score = 0;
};