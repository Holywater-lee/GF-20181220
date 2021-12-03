#include "Bullet.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"
#include "WIDTHHEIGHT.h"

// �Ѿ� �⺻ �ʱ�ȭ
Bullet::Bullet(const LoaderParams* pParams, SDL_RendererFlip isFlip) : SDLGameObject(pParams)
{
	tag = "Bullet";
	m_currentFrame = 0;
	flip = isFlip;
	createdTime = SDL_GetTicks();

	// �Ѿ��� ���� ���ϱ�
	Vector2D center(m_position.getX() + m_width / 2, m_position.getY() + m_height / 2);

	// ���Ϳ��� flip�� ���� ���� ���ϱ�
	Vector2D direction(center.getX() + (flip == SDL_FLIP_NONE ? 10 : -10), center.getY());
	m_velocity = direction - center;
	m_velocity.normalize();
	m_velocity *= moveSpeed;
}

// ȭ�� ������ ���� ��� clean ȣ��
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

// ������ �ð����κ��� lifeTime�� �����ٸ� clean ȣ��
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