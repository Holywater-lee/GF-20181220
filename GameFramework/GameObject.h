#pragma once
#include "LoaderParams.h"
#include "Vector2D.h"

class GameObject
{
public:
	virtual void draw() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void clean() = 0;
	virtual Vector2D GetPos() = 0;
	virtual void OnHit() = 0;
	virtual ~GameObject() {}

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;

protected:
	GameObject(const LoaderParams* pParams) {}
};