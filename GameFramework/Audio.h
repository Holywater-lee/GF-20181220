#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>

class Audio
{
public:
	static Audio* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new Audio();
		return s_pInstance;
	}
	~Audio() {}

	bool Init();
	bool LoadBGM(const char* pFileName, std::string nameID);
	bool LoadSFX(const char* pFileName, std::string nameID);
	void PlayBGM(std::string nameID);
	void StopBGM();
	void PlaySFX(std::string nameID);
	void RemoveSFX(std::string nameID);
	void RemoveBGM(std::string nameID);
	void SetVolume(int volume = 100);
	void Clean();

private:
	Audio() {}
	static Audio* s_pInstance;

	// ÃÖ´ë º¼·ý °ª
	const float maxVolume = 100;

	std::map<std::string, Mix_Music*> bgmMap;
	std::map<std::string, Mix_Chunk*> sfxMap;
};
typedef Audio TheAudio;