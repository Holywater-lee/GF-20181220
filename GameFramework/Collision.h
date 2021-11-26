#pragma once
#include "GameObject.h"
#include "WIDTHHEIGHT.h"
#include "MapManager.h"

class Collision
{
public:
	static bool onCollision(GameObject* a, GameObject* b)
	{
		float aLeft = a->GetPos().getX();
		float aTop = a->GetPos().getY();

		float bLeft = b->GetPos().getX();
		float bTop = b->GetPos().getY();

		if (aLeft >= bLeft + b->GetWidth())
			return false;
		if (aTop >= bTop + b->GetHeight())
			return false;
		if (aLeft + a->GetWidth() <= bLeft)
			return false;
		if (aTop + a->GetHeight() <= bTop)
			return false;

		return true;
	}

	static bool onCollision(SDL_Rect* a, GameObject* b)
	{
		float aLeft = a->x;
		float aTop = a->y;

		float bLeft = b->GetPos().getX();
		float bTop = b->GetPos().getY();

		if (aLeft >= bLeft + b->GetWidth())
			return false;
		if (aTop >= bTop + b->GetHeight())
			return false;
		if (aLeft + a->w <= bLeft)
			return false;
		if (aTop + a->h <= bTop)
			return false;

		return true;
	}
};