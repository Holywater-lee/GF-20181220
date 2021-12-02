#include "Audio.h"

#include <iostream>
#include "LoadFiles.h"

Audio* Audio::s_p_Instance = nullptr;

bool Audio::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "오류: 사운드 초기화 실패: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

void Audio::SetVolume(int volume)
{
	Mix_VolumeMusic(static_cast<int>((MIX_MAX_VOLUME / maxVolume) * volume));

	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		Mix_VolumeChunk(sfxMap[TheLoadFiles::Instance()->GetLoadedSfxMaps(i)], static_cast<int>((MIX_MAX_VOLUME / maxVolume) * volume));
	}
}

bool Audio::LoadBGM(const char* pFileName)
{
	bgm = Mix_LoadMUS(pFileName);
	if (bgm == nullptr)
	{
		std::cout << "오류: BGM 불러오기 실패: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

bool Audio::LoadSFX(const char* pFileName, std::string nameID)
{
	sfxMap[nameID] = Mix_LoadWAV(pFileName);
	if (sfxMap[nameID] == nullptr)
	{
		std::cout << "오류: 효과음 불러오기 실패: " << Mix_GetError() << std::endl;
		return false;
	}
}

void Audio::PlayBGM()
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm, -1);
	}
}

void Audio::StopBGM()
{
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

void Audio::PlaySFX(std::string nameID)
{
	Mix_PlayChannel(-1, sfxMap[nameID], 0);
}

void Audio::RemoveSFX(std::string nameID)
{
	Mix_FreeChunk(sfxMap[nameID]);
	sfxMap[nameID] = nullptr;
}

void Audio::Clean()
{
	Mix_FreeMusic(bgm);
	bgm = nullptr;

	Mix_CloseAudio();
	Mix_Quit();
}