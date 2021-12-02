#pragma once
#include "LoaderParams.h"
#include <string>

class UIObject
{
public:
	UIObject(const LoaderParams* pParams);
	virtual void Draw();

protected:
	int x;
	int y;
	int width;
	int height;
	std::string textureID;
};