#include "Text.h"
#include "Game.h"

Text::Text(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height)
{
}

void Text::Draw()
{
	SDL_Rect textLocation = { m_x, m_y, m_width, m_height };
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_texture, NULL, &textLocation);
}