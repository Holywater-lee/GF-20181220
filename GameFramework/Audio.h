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
	bool LoadBGM(const char* pFileName);
	bool LoadSFX(const char* pFileName, std::string nameID);
	void PlayBGM();
	void StopBGM();
	void PlaySFX(std::string nameID);
	void RemoveSFX(std::string nameID);
	void RemoveBGM();
	void SetVolume(int volume = 100);
	void Clean();

private:
	Audio() {}
	static Audio* s_pInstance;

	// ÃÖ´ë º¼·ý °ª
	const float maxVolume = 100;

	Mix_Music* bgm = nullptr;
	std::map<std::string, Mix_Chunk*> sfxMap;
};
typedef Audio TheAudio;