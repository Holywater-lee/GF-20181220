#pragma once

#include "SDLGameObject.h"

// 플레이어 공격 스트래티지 부모 클래스
class PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip) = 0;
};

// 근접 공격 스트래티지
class MeleeAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};

// 원거리 공격 스트래티지
class RangedAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};