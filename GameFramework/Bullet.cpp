#include "Bullet.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"
#include "WIDTHHEIGHT.h"

// 총알 기본 초기화
Bullet::Bullet(const LoaderParams* pParams, SDL_RendererFlip isFlip) : SDLGameObject(pParams)
{
	tag = "Bullet";
	m_currentFrame = 0;
	flip = isFlip;
	createdTime = SDL_GetTicks();

	// 총알의 센터 구하기
	Vector2D center(m_position.getX() + m_width / 2, m_position.getY() + m_height / 2);

	// 센터에서 flip에 따라 방향 정하기
	Vector2D direction(center.getX() + (flip == SDL_FLIP_NONE ? 10 : -10), center.getY());
	m_velocity = direction - center;
	m_velocity.normalize();
	m_velocity *= moveSpeed;
}

// 화면 밖으로 나간 경우 clean 호출
void Bullet::DeleteBecauseOutOfScreen()
{
	if (m_position.getX() + m_width <= 0 || m_position.getX() >= LEVEL_WIDTH)
	{
		this->clean();
	}
	if (m_position.getY() + m_height <= 0 || m_position.getY() >= LEVEL_HEIGHT)
	{
		this->clean();
	}
}

// 생성된 시간으로부터 lifeTime이 지났다면 clean 호출
void Bullet::DeleteBecauseLifeTime()
{
	if (SDL_GetTicks() >= createdTime + lifeTime)
	{
		this->clean();
	}
}

void Bullet::draw()
{
	SDLGameObject::draw();
}

void Bullet::update()
{
	SDLGameObject::update();
	DeleteBecauseOutOfScreen();
	DeleteBecauseLifeTime();
}

void Bullet::clean()
{
	SDLGameObject::clean();
}