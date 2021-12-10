#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Camera.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams) : GameObject(pParams), m_position(pParams->getX(), pParams->getY()), m_velocity(0, 0), m_acceleration(0, 0)
{
	m_width = pParams->getWidth();
	m_height = pParams->getHeight();
	m_textureID = pParams->getTextureID();
	m_currentRow = 0;
	m_currentFrame = 0;
}

void SDLGameObject::draw()
{
	TheTextureManager::Instance()->drawFrame(m_textureID, TheCam::Instance()->GetX(m_position.getX()), TheCam::Instance()->GetY(m_position.getY()), m_width, m_height, m_currentRow, m_currentFrame, flip);
}

void SDLGameObject::update()
{
	m_velocity += m_acceleration;
	m_position += m_velocity;
}

// 오버라이드하여 사용할 함수, 피격당할 경우 호출, 기본형 정의
void SDLGameObject::OnHit(int amount)
{

}

// isActive 변수를 두어 false로 변경해줌, false인 경우 Game이 삭제함
void SDLGameObject::clean()
{
	isActive = false;
}