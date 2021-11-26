#include "Ball.h"
#include "InputHandler.h"
#include "Vector2D.h"
#include "Game.h"
#include "WIDTHHEIGHT.h"

Ball::Ball(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Ball";
	m_currentFrame = 0;

	Vector2D center(m_position.getX() + m_width / 2, m_position.getY() + m_height / 2);

	Vector2D* direction = TheInputHandler::Instance()->getMousePosition();
	m_velocity = (*direction - center);
	m_velocity.normalize();
	m_velocity *= moveSpeed;
}

void Ball::DeleteBecauseOutOfScreen()
{
	if (m_position.getX() + m_width <= 0 || m_position.getX() >= LEVEL_WIDTH)
	{
		printf("ȭ�� ��Ż(X��): ");
		this->clean();
	}
	if (m_position.getY() + m_height <= 0 || m_position.getY() >= LEVEL_HEIGHT)
	{
		printf("ȭ�� ��Ż(Y��): ");
		this->clean();
	}
}

void Ball::draw()
{
	SDLGameObject::draw();
}

void Ball::update(float deltaTime)
{
	DeleteBecauseOutOfScreen();
	SDLGameObject::update(deltaTime);
	//CheckCollision(); // Game���� ���� �̵�
}

void Ball::clean()
{
	printf("��(�Ѿ�) ������!\n");
	SDLGameObject::clean();
}