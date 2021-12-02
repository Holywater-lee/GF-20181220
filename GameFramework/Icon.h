#pragma once
#include "UIObject.h"

class Icon : public UIObject
{
public:
	Icon(const LoaderParams* pParams) : UIObject(pParams), originalX(x) {}
	virtual void Draw()
	{
		UIObject::Draw();
	}

	void SetX(int amount)
	{
		x = amount;
	}

	int GetOriginalX()
	{
		return originalX;
	}

	int GetX()
	{
		return x;
	}

private:
	int originalX;
};