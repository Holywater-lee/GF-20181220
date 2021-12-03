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

private:
	UIManager();

	UIObject* hpBar;
	UIObject* fakeHpBar;
	UIObject* weaponIcon;

	const int hpBarMaxAmount = 5;
	int targetHpAmount = 0;
	int weaponIconOriginalX = 0;

	static UIManager* s_pInstance;
};
typedef UIManager TheUI;