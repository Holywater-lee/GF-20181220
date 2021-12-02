#include "Audio.h"
#include <iostream>

Audio* Audio::s_pInstance = nullptr;

bool Audio::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "����: ���� �ʱ�ȭ ����: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

bool Audio::LoadBGM(const char* pFileName)
{
	bgm = Mix_LoadMUS(pFileName);
	if (bgm == nullptr)
	{
		std::cout << "����: BGM �ҷ����� ����: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

bool Audio::LoadSFX(const char* pFileName, std::string nameID)
{
	sfxMap[nameID] = Mix_LoadWAV(pFileName);
	if (sfxMap[nameID] == nullptr)
	{
		std::cout << "����: ȿ���� �ҷ����� ����: " << Mix_GetError() << std::endl;
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

	Mix_Quit();
}