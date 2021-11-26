#pragma once
#include "SDLGameObject.h"
#include <vector>

class Ball : public SDLGameObject
{
public:
	Ball(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();

private:
	int moveSpeed = 6;

	void DeleteBecauseOutOfScreen();
};