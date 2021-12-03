#include "Text.h"
#include "Game.h"
#include "Camera.h"

Text::Text(int x, int y, int width, int height, bool isUIText) : m_x(x), m_y(y), m_width(width), m_height(height), isUI(isUIText)
{
}

// 텍스트의 위치를 그려줄 함수
void Text::Draw()
{
	SDL_Rect textLocation;
	if (isUI) // 만약 UI라면 화면에 고정시킨다
	{
		textLocation = { m_x, m_y, m_width, m_height };
	}
	else // UI가 아니라면 화면에 고정시키지 않고 카메라 움직임에 따라 상대적으로 위치가 변경된다
	{
		textLocation = { TheCam::Instance()->GetX(m_x), TheCam::Instance()->GetY(m_y), m_width, m_height };
	}
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_texture, NULL, &textLocation);
}