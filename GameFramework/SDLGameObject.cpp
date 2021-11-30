#include "SDLGameObject.h"
#include "Game.h"
#include "TextureManager.h"
#include "Camera.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) : GameObject(pParams), m_position(pParams->getX(), pParams->getY()), m_velocity(0, 0), m_acceleration(0, 0)
{
	m_width = pParams->getWidth();
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();
	m_currentRow = 0;
	m_currentFrame = 0;
	m_angle = 0;
}

void SDLGameObject::draw()
{
	TheTextureManager::Instance()->drawFrame(m_textureID, TheCam::Instance()->GetX(m_position.getX()), TheCam::Instance()->GetY(m_position.getY()), m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), flip, m_angle);
}

void SDLGameObject::update()
{
	m_velocity += m_acceleration;
	m_position += m_velocity;
}

void SDLGameObject::OnHit()
{

}

void SDLGameObject::clean()
{
	isActive = false;
}