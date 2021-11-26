#include "Enemy.h"
#include "ScoreManager.h"

Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Enemy";
	m_currentFrame = 0;
	life = 1;
	flip = SDL_FLIP_HORIZONTAL;
}

void Enemy::draw()
{
	SDLGameObject::draw();
}

void Enemy::OnHit()
{
	if (!isDead)
	{
		life--;
		printf("적 맞음 (라이프: %d)\n", life);

		if (life <= 0)
		{
			printf("적 사망 (라이프: %d)\n", life);
			TheScore::Instance()->AddScore(1);
			deadTime = SDL_GetTicks();
			isDead = true;
		}
	}
}

void Enemy::update(float deltaTime)
{
	if (isDead)
	{
		m_currentFrame = 1 + ((SDL_GetTicks() - deadTime) / 100) % 4;
		if (m_currentFrame == 4)
		{
			this->clean();
		}
	}
	//SDLGameObject::update(deltaTime);
}

void Enemy::clean()
{
	printf("적 삭제됨!\n");
	SDLGameObject::clean();
}