#pragma once
#include "SDL.h"

class Text
{
public:
	Text(int x, int y, int width, int height, bool isUIText = true);
	~Text() {}
	void Draw();
	void SetTexture(SDL_Texture* tex) { m_texture = tex; }
	void FreeTexture() { m_texture = NULL; }
private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	bool isUI = false;

	SDL_Texture* m_texture;
};