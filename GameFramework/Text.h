#pragma once
#include "SDL.h"

class Text
{
public:
	Text(int x, int y, int width, int height, bool isUIText = true);
	~Text() {}
	void Draw();
	void clean() { isActive = false; }
	void SetTexture(SDL_Texture* tex) { m_texture = tex; }
	void FreeTexture() { m_texture = NULL; }
	bool GetIsActive() { return isActive; }

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	bool isUI = false;
	bool isActive = true;

	SDL_Texture* m_texture;
};