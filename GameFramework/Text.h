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
	bool GetIsActive() const { return isActive; }

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	bool isUI = false; // true라면 카메라와 위치 무관, false라면 카메라에 상대적 위치
	bool isActive = true;

	SDL_Texture* m_texture = nullptr;
};