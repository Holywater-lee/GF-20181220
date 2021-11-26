#pragma once
#include "GameObject.h"
#include <vector>

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
};