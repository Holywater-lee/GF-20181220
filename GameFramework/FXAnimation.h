#pragma once
#include "SDLGameObject.h"

class FXAnimation : public SDLGameObject
{
public:
	FXAnimation(const LoaderParams* pParams, int sdlgetticks, int p_duration, int startRow = 0, int startFrame = 0, bool flipped = false, int frame = 4);
	virtual void update();

protected:
	int createdTime = 0;
	int duration = 0;
	int remainTime = 0;
	int totalFrame = 0;
	int startFrame = 0;

	void DeleteTimer();
};