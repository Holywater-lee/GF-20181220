#pragma once
#include "TextureManager.h"
#include "Game.h"
#include <string>

class ScrollingBackGround : public SDLGameObject
{
public:
	ScrollingBackGround(const LoaderParams* pParams, int moveSpeed) : SDLGameObject(pParams)
	{
		m_velocity.setX(moveSpeed);
	}
	virtual void draw()
	{
		TheTextureManager::Instance()->draw(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height, TheGame::Instance()->getRenderer());
	}
	virtual void update(float deltaTime)
	{
		m_position -= m_velocity;
		if (m_position.getX() + m_width < 0) m_position.setX(m_width);
	}
};