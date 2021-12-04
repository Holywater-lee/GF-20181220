#include "PlayerAttackStrategy.h"
#include "Game.h"
#include "Camera.h"
#include "Collision.h"
#include "FXAnimation.h"
#include "Bullet.h"
#include "Audio.h"

// 근접공격 스트래티지
void MeleeAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	// 카메라 위치를 이동시켜 타격감? 구현
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? 2 : -2, 20);
	// 오디오 재생
	TheAudio::Instance()->PlaySFX("SwordSFX");
	// 공격 위치 설정
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() + 2;
	// 실제 공격 적용
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
	// 공격 이펙트 오브젝트 생성
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXSword"), SDL_GetTicks(), 350, 1, flip == SDL_FLIP_HORIZONTAL));
}

// 원거리 공격 스트래티지
void RangedAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	// 카메라 위치 이동
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? -2 : 2, -5);
	// 오디오 재생
	TheAudio::Instance()->PlaySFX("GunSFX");
	// 이펙트 위치 설정
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() - 12;

	// 총알 오브젝트 생성
	TheGame::Instance()->CreateGameObject(new Bullet(new LoaderParams(obj->GetPos().getX() + obj->GetWidth() / 2 - 4, obj->GetPos().getY() + obj->GetHeight() / 2 - 2, 8, 8, "Bullet"), flip));
	// 공격 이펙트 오브젝트 생성
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXGun"), SDL_GetTicks(), 350, 0, flip == SDL_FLIP_HORIZONTAL));
}