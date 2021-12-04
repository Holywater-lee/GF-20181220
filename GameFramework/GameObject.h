#pragma once
#include "LoaderParams.h"
#include "Vector2D.h"

class GameObject
{
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;
	virtual Vector2D GetPos() const = 0;
	virtual void OnHit(int amount) = 0;
	virtual bool GetIsActive() const = 0;
	virtual ~GameObject() {}

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

protected:
	GameObject(const LoaderParams* pParams) {}
};