#include "BossEnemy.h"
#include "ScoreManager.h"

// �ʱ�ȭ
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

// �ǰݴ��ϴ� �Լ�
void BossEnemy::OnHit(int amount)
{
	if (currentPattern != PatternState::DEAD)
	{
		life -= amount;

		if (life > maxLife) life = maxLife;

		if (life <= 0)
		{
			// ���ھ 200�� �߰�
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