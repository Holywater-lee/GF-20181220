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
	int moveSpeed = 6;

	void DeleteBecauseOutOfScreen();
};