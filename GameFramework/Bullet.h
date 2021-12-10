#pragma once
#include "SDLGameObject.h"

class Bullet : public SDLGameObject
{
public:
	Bullet(const LoaderParams* pParams, SDL_RendererFlip isFlip);
	virtual void update();

private:
	void DeleteBecauseOutOfScreen();
	void DeleteBecauseLifeTime();

	const int moveSpeed = 6;
	const int lifeTime = 500;
	int createdTime = 0;
};