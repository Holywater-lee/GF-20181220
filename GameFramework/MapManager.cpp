#include "MapManager.h"
#include "Game.h"
#include "Enemy.h"
#include "Potion.h"
#include "SDLGameObject.h"

MapManager* MapManager::s_pInstance = nullptr;

// �� ����
void MapManager::SetMap(int map_x, int map_y, int value)
{
	loadedTileMap[map_x][map_y] = value;
}

// �� ������Ʈ�� ����
void MapManager::CreateMap(int x, int y)
{
	for (int i = 0; i < MAP_MAX_HEIGHT; i++)
	{
		for (int k = 0; k < MAP_MAX_WIDTH; k++)
		{
			if (loadedTileMap[i][k] == 1)
			{
				TheGame::Instance()->CreateTileObject(new SDLGameObject(new LoaderParams(x + k * TILE_SIZE, y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, "Tile")));
			}
			else if (loadedTileMap[i][k] == 2)
			{
				TheGame::Instance()->CreateGameObject(new Potion(new LoaderParams(x + k * TILE_SIZE, y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, "Potion")));
			}
			else if (loadedTileMap[i][k] == 3)
			{
				TheGame::Instance()->CreateGameObject(new Enemy(new LoaderParams(x + k * TILE_SIZE, y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, "Enemy")));
			}
		}
	}
}

// �� ���� Ÿ���� �ִ��� �˻�, Enemy�� ���� �� ���������� �������� �ʰԲ� �ϴ� ���� ����
bool MapManager::IsTileThere(int x, int y)
{
	return loadedTileMap[y][x] == 1;
}

// ����
void MapManager::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}