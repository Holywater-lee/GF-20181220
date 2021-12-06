#include "EnemyBase.h"
#include <vector>
#include "Collision.h"
#include "MapManager.h"

#include "WIDTHHEIGHT.h"

// 충돌 체크
void EnemyBase::CheckCollision(int checkX, int checkY)
{
	if (!isGrounded)
	{
		m_acceleration.setY(GRAVITY);
	}

	// 근처의 타일 정보를 저장할 벡터
	std::vector<SDL_Rect> collisionRects;

	// 근처에 타일이 있다면 위의 collisionRects 벡터에 저장한다
	for (int i = (m_position.getY() - TILE_SIZE) / TILE_SIZE; i <= (m_position.getY() + (TILE_SIZE * checkY)) / TILE_SIZE; i++)
	{
		for (int k = (m_position.getX() - TILE_SIZE) / TILE_SIZE; k <= (m_position.getX() + (TILE_SIZE * checkX)) / TILE_SIZE; k++)
		{
			if (TheMap::Instance()->IsTileThere(k, i))
			{
				SDL_Rect tempRect = { k * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				collisionRects.emplace_back(std::move(tempRect));
			}
		}
	}

	float oldX = m_position.getX();

	for (int i = 0; i < collisionRects.size(); i++)
	{
		if (Collision::onCollision(&collisionRects[i], this))
		{
			if (m_position.getY() + m_height != collisionRects[i].y && m_position.getY() != collisionRects[i].y + collisionRects[i].h)
			{
				if (m_velocity.getX() > 0 && m_position.getX() + m_width > collisionRects[i].x && m_position.getX() + m_width < collisionRects[i].x + collisionRects[i].w)
				{
					m_position.setX(collisionRects[i].x - m_width);
				}
				else if (m_velocity.getX() < 0 && m_position.getX() < collisionRects[i].x + collisionRects[i].w && m_position.getX() > collisionRects[i].x)
				{
					m_position.setX(collisionRects[i].x + collisionRects[i].w);
				}
				m_velocity.setX(0);
			}

			if (m_position.getX() + m_width != collisionRects[i].x && m_position.getX() != collisionRects[i].x + collisionRects[i].w)
			{
				if (m_velocity.getY() > 0 && m_position.getY() + m_height > collisionRects[i].y && m_position.getY() + m_height < collisionRects[i].y + collisionRects[i].h)
				{
					isGrounded = true;
					m_position.setX(oldX);
					m_position.setY(collisionRects[i].y - m_height);
				}
				else if (m_velocity.getY() < 0 && m_position.getY() < collisionRects[i].y + collisionRects[i].h && m_position.getY() > collisionRects[i].y)
				{
					m_position.setX(oldX);
					m_position.setY(collisionRects[i].y + collisionRects[i].h);
					m_velocity.setY(0);
				}

				m_acceleration.setY(0);
				m_velocity.setY(0);
			}
		}
	}

	m_position.setY(m_position.getY() + 1);
	int count = 0;
	for (auto& tile : collisionRects)
	{
		if (Collision::onCollision(&tile, this))
		{
			count++;
		}
	}
	m_position.setY(m_position.getY() - 1);
	if (count == 0)
	{
		isGrounded = false;
	}
}