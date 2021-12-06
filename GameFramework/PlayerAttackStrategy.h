#pragma once

#include "SDLGameObject.h"

// �÷��̾� ���� ��Ʈ��Ƽ�� �θ� Ŭ����
class PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip) = 0;
	virtual ~PlayerAttackStrategy() {}
};

// ���� ���� ��Ʈ��Ƽ��
class MeleeAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
	virtual ~MeleeAttackStrategy() {}
};

// ���Ÿ� ���� ��Ʈ��Ƽ��
class RangedAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
	virtual ~RangedAttackStrategy() {}
};