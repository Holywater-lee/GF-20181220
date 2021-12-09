#pragma once
#include "UIObject.h"

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
	void Clean();
	void RefreshHPBar(int amount);
	void SetWeaponIcon(std::string nameID);
	void ShakeIcon();

	void SetGameOverUI(bool win = true);

private:
	UIManager();

	UIObject* hpBar = nullptr;
	UIObject* fakeHpBar = nullptr;
	UIObject* weaponIcon = nullptr;
	UIObject* gameOverUI = nullptr;

	const int hpBarMaxAmount = 5;
	int targetHpAmount = 0;
	int weaponIconOriginalX = 0;

	static UIManager* s_pInstance;
};
typedef UIManager TheUI;