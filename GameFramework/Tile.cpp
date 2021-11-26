#include "Tile.h"

Tile::Tile(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	tag = "Tile";
}

void Tile::draw()
{
	SDLGameObject::draw();
}

void Tile::update(float deltaTime)
{

}

void Tile::clean()
{
	SDLGameObject::clean();
}