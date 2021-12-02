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
	void Clean();

private:
	Audio() {}
	static Audio* s_pInstance;
	Mix_Music* bgm;
	std::map<std::string, Mix_Chunk*> sfxMap;
};
typedef Audio TheAudio;