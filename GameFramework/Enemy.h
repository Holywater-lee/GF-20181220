#pragma once
#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
public:
	Enemy(const LoaderParams* pParams);
	virtual void draw();
	virtual void update(float deltaTime);
	virtual void clean();
	virtual void OnHit();

private:
	bool isDead = false;
	int deadTime;
};