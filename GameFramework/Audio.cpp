#include "Audio.h"

#include <iostream>
#include "LoadFiles.h"

Audio* Audio::s_pInstance = nullptr;

// ����� �ʱ�ȭ
bool Audio::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "����: ���� �ʱ�ȭ ����: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

// ���� ����
void Audio::SetVolume(int volume)
{
	// Mix_VolumeMusic�� �ִ밪�� 128������ �ִ밪�� 100���� ����
	int setVolume = static_cast<int>((MIX_MAX_VOLUME / maxVolume) * volume);
	// ����� ���� ����
	Mix_VolumeMusic(setVolume);

	// ��� ȿ������ ��ȸ�ϸ� ���� ����
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		Mix_VolumeChunk(sfxMap[TheLoadFiles::Instance()->GetLoadedSfxMaps(i)], setVolume);
	}
}

// ����� �ҷ�����
bool Audio::LoadBGM(const char* pFileName, std::string nameID)
{
	bgmMap[nameID] = Mix_LoadMUS(pFileName);
	if (bgmMap[nameID] == nullptr)
	{
		std::cout << "����: BGM �ҷ����� ����: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

// ȿ���� �ҷ�����
bool Audio::LoadSFX(const char* pFileName, std::string nameID)
{
	sfxMap[nameID] = Mix_LoadWAV(pFileName);
	if (sfxMap[nameID] == nullptr)
	{
		std::cout << "����: ȿ���� �ҷ����� ����: " << Mix_GetError() << std::endl;
		return false;
	}
}

// ����� ���
void Audio::PlayBGM(std::string nameID)
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgmMap[nameID], -1); // -1�� ���ѹݺ����
	}
}

// ����� ����
void Audio::StopBGM()
{
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

// nameID�� �ش��ϴ� ȿ���� ���
void Audio::PlaySFX(std::string nameID)
{
	Mix_PlayChannel(-1, sfxMap[nameID], 0);
}

// ȿ���� ����
void Audio::RemoveSFX(std::string nameID)
{
	Mix_FreeChunk(sfxMap[nameID]);
	sfxMap[nameID] = nullptr;
}

// ����� ����
void Audio::RemoveBGM(std::string nameID)
{
	Mix_FreeMusic(bgmMap[nameID]);
	bgmMap[nameID] = nullptr;
}

// Ŭ��
void Audio::Clean()
{
	Mix_CloseAudio();
	Mix_Quit();

	delete s_pInstance;
	s_pInstance = nullptr;
}