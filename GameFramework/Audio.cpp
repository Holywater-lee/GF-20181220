#include "Audio.h"

#include <iostream>
#include "LoadFiles.h"

Audio* Audio::s_pInstance = nullptr;

// 오디오 초기화
bool Audio::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "오류: 사운드 초기화 실패: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

// 볼륨 조절
void Audio::SetVolume(int volume)
{
	// Mix_VolumeMusic의 최대값은 128이지만 최대값을 100으로 설정
	int setVolume = static_cast<int>((MIX_MAX_VOLUME / maxVolume) * volume);
	// 배경음 볼륨 조절
	Mix_VolumeMusic(setVolume);

	// 모든 효과음을 순회하며 볼륨 조절
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		Mix_VolumeChunk(sfxMap[TheLoadFiles::Instance()->GetLoadedSfxMaps(i)], setVolume);
	}
}

// 배경음 불러오기
bool Audio::LoadBGM(const char* pFileName, std::string nameID)
{
	bgmMap[nameID] = Mix_LoadMUS(pFileName);
	if (bgmMap[nameID] == nullptr)
	{
		std::cout << "오류: BGM 불러오기 실패: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

// 효과음 불러오기
bool Audio::LoadSFX(const char* pFileName, std::string nameID)
{
	sfxMap[nameID] = Mix_LoadWAV(pFileName);
	if (sfxMap[nameID] == nullptr)
	{
		std::cout << "오류: 효과음 불러오기 실패: " << Mix_GetError() << std::endl;
		return false;
	}
}

// 배경음 재생
void Audio::PlayBGM(std::string nameID)
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgmMap[nameID], -1); // -1은 무한반복재생
	}
}

// 배경음 중지
void Audio::StopBGM()
{
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

// nameID에 해당하는 효과음 재생
void Audio::PlaySFX(std::string nameID)
{
	Mix_PlayChannel(-1, sfxMap[nameID], 0);
}

// 효과음 삭제
void Audio::RemoveSFX(std::string nameID)
{
	Mix_FreeChunk(sfxMap[nameID]);
	sfxMap[nameID] = nullptr;
}

// 배경음 삭제
void Audio::RemoveBGM(std::string nameID)
{
	Mix_FreeMusic(bgmMap[nameID]);
	bgmMap[nameID] = nullptr;
}

// 클린
void Audio::Clean()
{
	Mix_CloseAudio();
	Mix_Quit();

	delete s_pInstance;
	s_pInstance = nullptr;
}