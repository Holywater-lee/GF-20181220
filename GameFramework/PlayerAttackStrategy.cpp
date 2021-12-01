#include "PlayerAttackStrategy.h"
#include "Game.h"
#include "Camera.h"
#include "Collision.h"
#include "FXAnimation.h"
#include "Bullet.h"

void MeleeAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? 2 : -2, 20);
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() + 2;
	for (const auto& enemy : TheGame::Instance()->GetGameObjects())
	{
		if (Collision::onCollision(atkArea, enemy))
		{
			if (dynamic_cast<SDLGameObject*>(enemy)->GetTag() == "Enemy")
			{
				enemy->OnHit();
			}
		}
	}
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXSword"), SDL_GetTicks(), 350, 1, flip == SDL_FLIP_HORIZONTAL));
}

void RangedAttackStrategy::AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip)
{
	TheCam::Instance()->AddForce(flip == SDL_FLIP_NONE ? -2 : 2, -5);
	atkArea->x = obj->GetPos().getX() + obj->GetWidth() / 2 + (flip == SDL_FLIP_NONE ? 10 : -10 - 32);
	atkArea->y = obj->GetPos().getY() - 12;

	TheGame::Instance()->CreateGameObject(new Bullet(new LoaderParams(obj->GetPos().getX() + obj->GetWidth() / 2 - 4, obj->GetPos().getY() + obj->GetHeight() / 2 - 2, 8, 8, "Bullet"), flip));
	TheGame::Instance()->CreateGameObject(new FXAnimation(new LoaderParams(atkArea->x, atkArea->y, 32, 64, "FXGun"), SDL_GetTicks(), 350, 0, flip == SDL_FLIP_HORIZONTAL));
}