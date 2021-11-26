#pragma once
#include "SDLGameObject.h"

class FXAnimation : public SDLGameObject
{
public:
	FXAnimation(const LoaderParams* pParams, int sdlgetticks, int p_duration, int row = 0, bool flipped = false);
	virtual void draw();
	virtual void update();
	virtual void clean();

protected:
	int createdTime = 0;
	int duration = 0;
	int remainTime = 0;

	void DeleteTimer();
};