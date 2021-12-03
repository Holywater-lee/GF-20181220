#pragma once

#include "SDLGameObject.h"

// �÷��̾� ���� ��Ʈ��Ƽ�� �θ� Ŭ����
class PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip) = 0;
};

// ���� ���� ��Ʈ��Ƽ��
class MeleeAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};

// ���Ÿ� ���� ��Ʈ��Ƽ��
class RangedAttackStrategy : public PlayerAttackStrategy
{
public:
	virtual void AttackAction(SDLGameObject* obj, SDL_Rect* atkArea, SDL_RendererFlip flip);
};