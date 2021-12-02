#include "UIManager.h"
#include <SDL.h>
#include "WIDTHHEIGHT.h"
#include "GetRandom.h"

UIManager* UIManager::s_pInstance = nullptr;

UIManager::UIManager()
{
	fakeHpBar = new HPBar(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HPDark"));
	hpBar = new HPBar(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HP"));
	weaponIcon = new Icon(new LoaderParams(SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48, 32, 32, "IconGun"));
}

void UIManager::Init()
{
	RefreshHPBar(hpBarMaxAmount);
	fakeHpBar->SetWidth(targetHpAmount);
	SetWeaponIcon("IconGun");
}

void UIManager::Update()
{
	if (fakeHpBar->GetWidth() != targetHpAmount)
	{
		fakeHpBar->SetWidth(Lerp(fakeHpBar->GetWidth(), targetHpAmount, 0.003f));
	}
	if (weaponIcon->GetX() != weaponIcon->GetOriginalX())
	{
		weaponIcon->SetX(Lerp(weaponIcon->GetOriginalX(), weaponIcon->GetX(), 0.1f));
	}
}

void UIManager::Draw()
{
	fakeHpBar->Draw();
	hpBar->Draw();

	if (weaponIcon != nullptr)
		weaponIcon->Draw();
}

void UIManager::RefreshHPBar(int amount)
{
	targetHpAmount = static_cast<int>((float)amount * 128 / hpBarMaxAmount);
	hpBar->SetWidth(targetHpAmount);
}

void UIManager::SetWeaponIcon(std::string nameID)
{
	if (weaponIcon != nullptr)
	{
		delete weaponIcon;
		weaponIcon = nullptr;
	}
	weaponIcon = new Icon(new LoaderParams(SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48, 32, 32, nameID));
}

void UIManager::ShakeIcon()
{
	int randomInt = GetRandom::GetRandomInt(0,1);
	weaponIcon->SetX(weaponIcon->GetOriginalX() + 32 * (randomInt == 0 ? 1 : -1));
}

int UIManager::Lerp(int a, int b, float dampTime)
{
	return (a + (b - a) * dampTime);
}

void UIManager::Clean()
{
	if (hpBar != nullptr)
	{
		delete hpBar;
		hpBar = nullptr;
	}
	if (fakeHpBar != nullptr)
	{
		delete fakeHpBar;
		fakeHpBar = nullptr;
	}
	if (weaponIcon != nullptr)
	{
		delete weaponIcon;
		weaponIcon = nullptr;
	}
}