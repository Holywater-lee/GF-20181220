#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>

class Audio
{
public:
	static Audio* Instance()
	{
		if (s_p_Instance == nullptr) s_p_Instance = new Audio();
		return s_p_Instance;
	}
	~Audio() {}

	bool Init();
	bool LoadBGM(const char* pFileName);
	bool LoadSFX(const char* pFileName, std::string nameID);
	void PlayBGM();
	void StopBGM();
	void PlaySFX(std::string nameID);
	void RemoveSFX(std::string nameID);
	void SetVolume(int volume = 100);
	void Clean();

private:
	Audio() {}
	static Audio* s_p_Instance;

	float maxVolume = 100;

	Mix_Music* bgm;
	std::map<std::string, Mix_Chunk*> sfxMap;
};
typedef Audio TheAudio;