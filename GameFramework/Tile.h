#pragma once
#include "SDLGameObject.h"

class Tile : public SDLGameObject
{
public:
	Tile(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
};