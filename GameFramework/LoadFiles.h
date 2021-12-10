#pragma once
#include <vector>
#include <string>
#include "WIDTHHEIGHT.h"
#include "MapManager.h"
#include <fstream>
#include <iostream>

using namespace std;

class LoadFiles
{
public:
	~LoadFiles() {}
	static LoadFiles* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new LoadFiles();
		return s_pInstance;
	}

	// ������ �ҷ��� ���� �о� �����ϴ� �Լ�
	bool Load()
	{
		string temp_string;
		int temp_int;

		// �ؽ��� �ּ�
		ifstream texture("Assets/Files/TexturesInit.txt");
		if (!texture) return false;
		// �ؽ��İ� ���� ������ ������
		while (texture.peek() != EOF)
		{
			getline(texture, temp_string);
			loadedTextures.emplace_back(temp_string);
			cout << "�ؽ��� �ּ� �ҷ�����: " << loadedTextures.back() << endl;
		}
		texture.close();

		// �ؽ��� ��
		ifstream texMap("Assets/Files/TextureMapsInit.txt");
		if (!texMap) return false;
		while (texMap.peek() != EOF)
		{
			getline(texMap, temp_string);
			loadedTexMaps.emplace_back(temp_string);
			cout << "�ؽ��� �� �ҷ�����: " << loadedTexMaps.back() << endl;
		}
		texMap.close();

		// ����� �ּ�
		ifstream bgmFile("Assets/Files/BgmInit.txt");
		if (!bgmFile) return false;
		while (bgmFile.peek() != EOF)
		{
			getline(bgmFile, temp_string);
			loadedBGMFiles.emplace_back(temp_string);
			cout << "BGM �ּ� �ҷ�����: " << loadedBGMFiles.back() << endl;
		}
		bgmFile.close();

		// ����� ��
		ifstream bgmMap("Assets/Files/BgmMapsInit.txt");
		if (!bgmMap) return false;
		while (bgmMap.peek() != EOF)
		{
			getline(bgmMap, temp_string);
			loadedBGMMaps.emplace_back(temp_string);
			cout << "BGM �ּ� �ҷ�����: " << loadedBGMMaps.back() << endl;
		}
		bgmMap.close();

		// ȿ���� �ּ�
		ifstream sfxFile("Assets/Files/SfxInit.txt");
		if (!sfxFile) return false;
		while (sfxFile.peek() != EOF)
		{
			getline(sfxFile, temp_string);
			loadedSfxFiles.emplace_back(temp_string);
			cout << "���� �ּ� �ҷ�����: " << loadedSfxFiles.back() << endl;
		}
		sfxFile.close();

		// ���� ��
		ifstream sfxMap("Assets/Files/SfxMapsInit.txt");
		if (!sfxMap) return false;
		while (sfxMap.peek() != EOF)
		{
			getline(sfxMap, temp_string);
			loadedSfxMaps.emplace_back(temp_string);
			cout << "���� �� �ҷ�����: " << loadedSfxMaps.back() << endl;
		}
		sfxMap.close();

		// Ÿ�ϸ�
		ifstream tileMap("Assets/Files/MapInit.txt");
		if (!tileMap) return false;

		cout << "�� �ҷ����� ��: " << endl;
		for (int i = 0; i < MAP_MAX_HEIGHT; i++)
		{
			for (int k = 0; k < MAP_MAX_WIDTH; k++)
			{
				tileMap >> temp_int;
				// MapManager�� ����
				TheMap::Instance()->SetMap(i, k, temp_int);
				cout << temp_int;
			}
			cout << endl;
		}
		cout << "�� �ҷ����� ����" << endl;
		tileMap.close();

		return true;
	}

	string GetLoadedTextures(int index) { return loadedTextures[index]; }
	string GetLoadedTexMaps(int index) { return loadedTexMaps[index]; }
	string GetLoadedSfxFiles(int index) { return loadedSfxFiles[index]; }
	string GetLoadedSfxMaps(int index) { return loadedSfxMaps[index]; }
	string GetLoadedBGMFiles(int index) { return loadedBGMFiles[index]; }
	string GetLoadedBGMMaps(int index) { return loadedBGMMaps[index]; }

	size_t GetTexMapsSize() { return loadedTexMaps.size(); }
	size_t GetSfxMapsSize() { return loadedSfxMaps.size(); }
	size_t GetBGMMapsSize() { return loadedBGMMaps.size(); }

	void Clean();

private:
	LoadFiles() {}
	static LoadFiles* s_pInstance;

	vector<string> loadedTextures;
	vector<string> loadedTexMaps;
	vector<string> loadedSfxFiles;
	vector<string> loadedSfxMaps;
	vector<string> loadedBGMFiles;
	vector<string> loadedBGMMaps;
};
typedef LoadFiles TheLoadFiles;