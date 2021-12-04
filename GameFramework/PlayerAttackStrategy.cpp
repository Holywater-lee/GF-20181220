#include "PlayerAttackStrategy.h"
#include "Game.h"
#include "Camera.h"
#include "Collision.h"
#include "FXAnimation.h"
#include "Bullet.h"
#include "Audio.h"

// �������� ��Ʈ��Ƽ��
void MeleeAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	// ī�޶� ��ġ�� �̵����� Ÿ�ݰ�? ����
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? 2 : -2, 20);
	// ����� ���
	TheAudio::Instance()->PlaySFX("SwordSFX");
	// ���� ��ġ ����
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() + 2;
	// ���� ���� ����
	for (const auto& enemy : TheGame::Instance()->GetGameObjects())
	{
		if (Collision::onCollision(atkArea, enemy))
		{
			if (dynamic_cast<SDLGameObject*>(enemy)->GetTag() == "Enemy")
			{
				enemy->OnHit(2);
			}
		}
	}
	// ���� ����Ʈ ������Ʈ ����
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXSword"), SDL_GetTicks(), 350, 1, flip == SDL_FLIP_HORIZONTAL));
}

// ���Ÿ� ���� ��Ʈ��Ƽ��
void RangedAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	// ī�޶� ��ġ �̵�
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? -2 : 2, -5);
	// ����� ���
	TheAudio::Instance()->PlaySFX("GunSFX");
	// ����Ʈ ��ġ ����
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() - 12;

	// �Ѿ� ������Ʈ ����
	TheGame::Instance()->CreateGameObject(new Bullet(new LoaderParams(obj->GetPos().getX() + obj->GetWidth() / 2 - 4, obj->GetPos().getY() + obj->GetHeight() / 2 - 2, 8, 8, "Bullet"), flip));
	// ���� ����Ʈ ������Ʈ ����
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXGun"), SDL_GetTicks(), 350, 0, flip == SDL_FLIP_HORIZONTAL));
}