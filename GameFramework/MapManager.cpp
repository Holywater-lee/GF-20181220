#include "MapManager.h"
#include "Game.h"
#include "Enemy.h"

MapManager* MapManager::s_pInstance = nullptr;

void MapManager::SetMap(int map_x, int map_y, int value)
{
	loadedTileMap[map_x][map_y] = value;
}

void MapManager::CreateMap(int x, int y)
{
	for (int i = 0; i < MAP_MAX_HEIGHT; i++)
	{
		for (int k = 0; k < MAP_MAX_WIDTH; k++)
		{
			if (loadedTileMap[i][k] == 1)
			{
				TheGame::Instance()->CreateTileObject(new Tile(new LoaderParams(x + k * TILE_SIZE, y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, "Tile")));
			}
			else if (loadedTileMap[i][k] == 3)
			{
				TheGame::Instance()->CreateGameObject(new Enemy(new LoaderParams(x + k * TILE_SIZE, y + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, "Enemy")));
			}
		}
	}
}

bool MapManager::IsTileThere(int x, int y)
{
	return loadedTileMap[x][y] == 1;
}