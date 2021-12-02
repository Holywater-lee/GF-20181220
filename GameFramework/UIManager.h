#pragma once
#include "HPBar.h"

class UIManager
{
public:
	static UIManager* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new UIManager();
		return s_pInstance;
	}
	~UIManager() {}

	void Init();
	void Update();
	void Draw();
	void RefreshHPBar(int amount);

private:
	UIManager();

	int Lerp(int a, int b, float dampTime);

	HPBar* hpBar;
	HPBar* fakeHpBar;
	int hpBarMaxAmount = 5;
	int targetHpAmount = 0;

	static UIManager* s_pInstance;
};
typedef UIManager TheUI;