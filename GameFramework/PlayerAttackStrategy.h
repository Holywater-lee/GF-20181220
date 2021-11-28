#pragma once

#include "SDLGameObject.h"

class PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip) = 0;
};

class MeleeAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};

class RangedAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};