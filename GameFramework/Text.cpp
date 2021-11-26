#include "Text.h"
#include "Game.h"
#include "Camera.h"

Text::Text(int x, int y, int width, int height, bool isUIText) : m_x(x), m_y(y), m_width(width), m_height(height), isUI(isUIText)
{
}

void Text::Draw()
{
	SDL_Rect textLocation;
	if (isUI)
	{
		textLocation = { m_x, m_y, m_width, m_height };
	}
	else
	{
		textLocation = { TheCam::Instance()->GetX(m_x), TheCam::Instance()->GetY(m_y), m_width, m_height };
	}
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_texture, NULL, &textLocation);
}