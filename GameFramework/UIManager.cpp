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

// ���� �ʱⰪ�� �ʱ�ȭ, ����� ��� �� ó�� �ʱ�ȭ�� ���...
void UIManager::Init()
{
	RefreshHPBar(hpBarMaxAmount);
	fakeHpBar->SetWidth(targetHpAmount);
	SetWeaponIcon("IconGun");
}

void UIManager::Update()
{
	// fakeHpBar�� �ʺ��� ������ ���� �ٸ��ٸ� Lerp �������� �ε巯�� �̵��� ó��
	if (fakeHpBar->GetWidth() != targetHpAmount)
	{
		fakeHpBar->SetWidth(Utility::Lerp(fakeHpBar->GetWidth(), targetHpAmount, 0.001f));
	}
	// weaponIcon�� x���� ���� ��ġ�� �ٸ��ٸ� Lerp �������� ���� ��ġ�� �̵�
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

// hpBar�� �����ϰ� fakeHpBar�� ���󰡾� �� ��ǥġ�� ����ϴ� �Լ�
void UIManager::RefreshHPBar(int amount)
{
	targetHpAmount = static_cast<int>((float)amount * 128 / hpBarMaxAmount);
	hpBar->SetWidth(targetHpAmount);
}

// weaponIcon�� �ؽ��� ���̵� ���� ���� �����ϴ� �Լ�
void UIManager::SetWeaponIcon(std::string nameID)
{
	if (weaponIcon != nullptr)
	{
		delete weaponIcon;
		weaponIcon = nullptr;
	}
	weaponIcon = new UIObject(new LoaderParams(SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48, 32, 32, nameID));
}

// weaponIcon�� �¿� �����ϰ� �̵���Ű�� �Լ�
void UIManager::ShakeIcon()
{
	int randomInt = Utility::GetRandomInt(0,1);
	weaponIcon->SetX(weaponIconOriginalX + 32 * (randomInt == 0 ? 1 : -1));
}

// Ŭ��
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