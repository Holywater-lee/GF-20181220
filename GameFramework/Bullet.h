#pragma once
#include "SDLGameObject.h"
#include <vector>

class Bullet : public SDLGameObject
{
public:
	Bullet(const LoaderParams* pParams, SDL_RendererFlip isFlip);
	virtual void draw();
	virtual void update();
	virtual void clean();

private:
	void DeleteBecauseOutOfScreen();
	void DeleteBecauseLifeTime();

	const int moveSpeed = 6;
	const int lifeTime = 500;
	int createdTime = 0;
};