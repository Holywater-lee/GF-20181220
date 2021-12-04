#include "Potion.h"
#include "Audio.h"
#include <math.h>

Potion::Potion(const LoaderParams* pParams) : SDLGameObject(pParams), originalYPos(pParams->getY())
{
	tag = "Potion";
}

void Potion::draw()
{
	SDLGameObject::draw();
}

void Potion::update()
{
	m_position.setY(originalYPos + 4 * sin(SDL_GetTicks() / 100));
}

void Potion::clean()
{
	TheAudio::Instance()->PlaySFX("PotionSFX");
	SDLGameObject::clean();
}