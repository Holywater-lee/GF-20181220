#pragma once
#include "SDLGameObject.h"

class Potion : public SDLGameObject
{
public:
	Potion(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();

	int GetHealAmount() const { return healAmount; }
private:
	const int healAmount = 1;
	const int originalYPos = 0;
};