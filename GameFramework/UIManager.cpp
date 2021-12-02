#include "UIManager.h"
#include <SDL.h>
#include "WIDTHHEIGHT.h"

UIManager* UIManager::s_pInstance = nullptr;

UIManager::UIManager()
{
	fakeHpBar = new HPBar(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HPDark"));
	hpBar = new HPBar(new LoaderParams(16, SCREEN_HEIGHT - 48, 128, 32, "HP"));
}

void UIManager::Init()
{
	RefreshHPBar(hpBarMaxAmount);
	fakeHpBar->SetWidth(targetHpAmount);
}

void UIManager::Update()
{
	if (fakeHpBar->GetWidth() != targetHpAmount)
	{
		fakeHpBar->SetWidth(Lerp(fakeHpBar->GetWidth(), targetHpAmount, 0.005f));
	}
}

void UIManager::Draw()
{
	fakeHpBar->Draw();
	hpBar->Draw();
}

void UIManager::RefreshHPBar(int amount)
{
	targetHpAmount = static_cast<int>((float)amount * 128 / hpBarMaxAmount);
	hpBar->SetWidth(targetHpAmount);
}

int UIManager::Lerp(int a, int b, float dampTime)
{
	return (a + (b - a) * dampTime);
}