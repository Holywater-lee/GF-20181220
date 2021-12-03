#pragma once
#include "LoaderParams.h"
#include <string>

class UIObject
{
public:
	UIObject(const LoaderParams* pParams);
	virtual void Draw();

	void SetX(int amount) { x = amount; }
	int GetX() { return x; }
	void SetWidth(int amount) { width = amount; }
	int GetWidth() { return width; }

protected:
	int x;
	int y;
	int width;
	int height;
	std::string textureID;
};