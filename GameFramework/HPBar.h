#pragma once
#include "UIObject.h"

class HPBar : public UIObject
{
public:
	HPBar(const LoaderParams* pParams) : UIObject(pParams) {}
	virtual void Draw()
	{
		UIObject::Draw();
	}

	void SetWidth(int amount)
	{
		width = amount;
	}

	int GetWidth()
	{
		return width;
	}
};