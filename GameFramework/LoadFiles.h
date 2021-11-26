#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "WIDTHHEIGHT.h"
#include "MapManager.h"

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

		ifstream texture("Assets/Files/TexturesInit.txt");
		if (!texture) return false;
		while (texture.peek() != EOF)
		{
			getline(texture, temp_string);
			loadedTextures.emplace_back(temp_string);
			cout << "텍스쳐 주소 불러오기: " << loadedTextures.back() << endl;
		}
		texture.close();

		ifstream texMap("Assets/Files/TextureMapsInit.txt");
		if (!texMap) return false;
		while (texMap.peek() != EOF)
		{
			getline(texMap, temp_string);
			loadedTexMaps.emplace_back(temp_string);
			cout << "텍스쳐 맵 불러오기: " << loadedTexMaps.back() << endl;
		}
		texMap.close();

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

	size_t GetTexMapsSize() { return loadedTexMaps.size(); }

private:
	LoadFiles() {}
	static LoadFiles* s_pInstance;

	vector<string> loadedTextures;
	vector<string> loadedTexMaps;
};
typedef LoadFiles TheLoadFiles;

LoadFiles* LoadFiles::s_pInstance = nullptr;