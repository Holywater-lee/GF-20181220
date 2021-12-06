#pragma once
#include "SDLGameObject.h"

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
	virtual void CheckCollision();
	bool isGrounded = true;
};