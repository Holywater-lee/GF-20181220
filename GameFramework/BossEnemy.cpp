#include "BossEnemy.h"
#include "ScoreManager.h"

// 초기화
BossEnemy::BossEnemy(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Enemy";
	life = 50;
	maxLife = life;
}

void BossEnemy::draw()
{
	SDLGameObject::draw();
}

// 피격당하는 함수
void BossEnemy::OnHit(int amount)
{
	if (currentPattern != PatternState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// 스코어에 200점 추가
			TheScore::Instance()->AddScore(200);
			//ChangeState(PatternState::DEAD);
		}
	}
}

void BossEnemy::update()
{

}

void BossEnemy::clean()
{
	SDLGameObject::clean();
}