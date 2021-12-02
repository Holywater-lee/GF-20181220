#pragma once
#include <vector>
#include <string>
#include "WIDTHHEIGHT.h"
#include "MapManager.h"
#include <fstream>

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

	bool Load()
	{
		string temp_string;
		int temp_int;

		// 텍스쳐 주소
		ifstream texture("Assets/Files/TexturesInit.txt");
		if (!texture) return false;
		while (texture.peek() != EOF)
		{
			getline(texture, temp_string);
			loadedTextures.emplace_back(temp_string);
			cout << "텍스쳐 주소 불러오기: " << loadedTextures.back() << endl;
		}
		texture.close();

		// 텍스쳐 맵
		ifstream texMap("Assets/Files/TextureMapsInit.txt");
		if (!texMap) return false;
		while (texMap.peek() != EOF)
		{
			getline(texMap, temp_string);
			loadedTexMaps.emplace_back(temp_string);
			cout << "텍스쳐 맵 불러오기: " << loadedTexMaps.back() << endl;
		}
		texMap.close();

		// 브금 주소 (1개만, 추후 수정 가능성 있음)
		ifstream bgmFile("Assets/Files/BgmInit.txt");
		if (!bgmFile) return false;
		while (bgmFile.peek() != EOF)
		{
			getline(bgmFile, temp_string);
			loadedBGM = temp_string;
			cout << "BGM 주소 불러오기: " << loadedBGM << endl;
		}
		bgmFile.close();

		// 사운드 주소
		ifstream sfxFile("Assets/Files/SfxInit.txt");
		if (!sfxFile) return false;
		while (sfxFile.peek() != EOF)
		{
			getline(sfxFile, temp_string);
			loadedSfxFiles.emplace_back(temp_string);
			cout << "사운드 주소 불러오기: " << loadedSfxFiles.back() << endl;
		}
		sfxFile.close();

		// 사운드 맵
		ifstream sfxMap("Assets/Files/SfxMapsInit.txt");
		if (!sfxMap) return false;
		while (sfxMap.peek() != EOF)
		{
			getline(sfxMap, temp_string);
			loadedSfxMaps.emplace_back(temp_string);
			cout << "사운드 맵 불러오기: " << loadedSfxMaps.back() << endl;
		}
		sfxMap.close();

		// 타일맵
		ifstream tileMap("Assets/Files/MapInit.txt");
		if (!tileMap) return false;

		cout << "맵 불러오는 중: " << endl;
		for (int i = 0; i < MAP_MAX_HEIGHT; i++)
		{
			for (int k = 0; k < MAP_MAX_WIDTH; k++)
			{
				tileMap >> temp_int;
				TheMap::Instance()->SetMap(i, k, temp_int);
				cout << temp_int;
			}
			cout << endl;
		}
		cout << "맵 불러오기 성공" << endl;
		tileMap.close();

		return true;
	}

	string GetLoadedTextures(int index) { return loadedTextures[index]; }
	string GetLoadedTexMaps(int index) { return loadedTexMaps[index]; }
	string GetLoadedSfxFiles(int index) { return loadedSfxFiles[index]; }
	string GetLoadedSfxMaps(int index) { return loadedSfxMaps[index]; }
	string GetLoadedBGM() { return loadedBGM; }

	size_t GetTexMapsSize() { return loadedTexMaps.size(); }
	size_t GetSfxMapsSize() { return loadedSfxMaps.size(); }

private:
	LoadFiles() {}
	static LoadFiles* s_pInstance;

	string loadedBGM;

	vector<string> loadedTextures;
	vector<string> loadedTexMaps;
	vector<string> loadedSfxFiles;
	vector<string> loadedSfxMaps;
};
typedef LoadFiles TheLoadFiles;