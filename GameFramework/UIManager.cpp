#include "UIManager.h"
#include <SDL.h>
#include "WIDTHHEIGHT.h"
#include "Utility.h"

UIManager* UIManager::s_pInstance = nullptr;

UIManager::UIManager()
{
	fakeHpBar = new UIObject(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HPDark"));
	hpBar = new UIObject(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HP"));
	weaponIcon = new UIObject(new LoaderParams(SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48, 32, 32, "IconGun"));
	weaponIconOriginalX = weaponIcon->GetX();
}

// 각종 초기값을 초기화, 재시작 기능 및 처음 초기화에 사용...
void UIManager::Init()
{
	RefreshHPBar(hpBarMaxAmount);
	fakeHpBar->SetWidth(targetHpAmount);
	SetWeaponIcon("IconGun");
}

void UIManager::Update()
{
	// fakeHpBar의 너비값이 설정된 값과 다르다면 Lerp 연산으로 부드러운 이동을 처리
	if (fakeHpBar->GetWidth() != targetHpAmount)
	{
		fakeHpBar->SetWidth(Utility::Lerp(fakeHpBar->GetWidth(), targetHpAmount, 0.001f));
	}
	// weaponIcon의 x값이 원래 위치와 다르다면 Lerp 연산으로 원래 위치로 이동
	if (weaponIcon->GetX() != weaponIconOriginalX)
	{
		weaponIcon->SetX(Utility::Lerp(weaponIconOriginalX, weaponIcon->GetX(), 0.05f));
	}
}

void UIManager::Draw()
{
	fakeHpBar->Draw();
	hpBar->Draw();

	if (weaponIcon != nullptr)
		weaponIcon->Draw();
}

// hpBar를 갱신하고 fakeHpBar가 따라가야 할 목표치를 계산하는 함수
void UIManager::RefreshHPBar(int amount)
{
	targetHpAmount = static_cast<int>((float)amount * 128 / hpBarMaxAmount);
	hpBar->SetWidth(targetHpAmount);
}

// weaponIcon을 텍스쳐 아이디 값에 따라 변경하는 함수
void UIManager::SetWeaponIcon(std::string nameID)
{
	if (weaponIcon != nullptr)
	{
		delete weaponIcon;
		weaponIcon = nullptr;
	}
	weaponIcon = new UIObject(new LoaderParams(SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48, 32, 32, nameID));
}

// weaponIcon을 좌우 랜덤하게 이동시키는 함수
void UIManager::ShakeIcon()
{
	int randomInt = Utility::GetRandomInt(0,1);
	weaponIcon->SetX(weaponIconOriginalX + 32 * (randomInt == 0 ? 1 : -1));
}

// 클린
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

	delete s_pInstance;
	s_pInstance = nullptr;
}