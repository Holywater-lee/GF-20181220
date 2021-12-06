#pragma once
#include "SDLGameObject.h"

// Enemy와 BossEnemy의 부모가 되는 클래스
class EnemyBase : public SDLGameObject
{
public:
	EnemyBase(const LoaderParams* pParams) : SDLGameObject(pParams) {}
	virtual void draw()
	{
		SDLGameObject::draw();
	}
	virtual void update()
	{
		SDLGameObject::update();
	}
	virtual void clean()
	{
		SDLGameObject::clean();
	}
	virtual void OnHit(int amount) = 0;

protected:
	void CheckCollision(int checkX, int checkY);
	bool isGrounded = true;
	int score = 0;
};