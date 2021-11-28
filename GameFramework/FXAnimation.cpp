#include "FXAnimation.h"

FXAnimation::FXAnimation(const LoaderParams* pParams, int sdlgetticks, int p_duration, int row, bool flipped) : SDLGameObject(pParams), createdTime(sdlgetticks), duration(p_duration)
{
	tag = "FX";
	flip = flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	remainTime = duration - (SDL_GetTicks() - createdTime);
	m_currentRow = row;
}

void FXAnimation::draw()
{
	SDLGameObject::draw();
}

void FXAnimation::update()
{
	DeleteTimer();
}

void FXAnimation::DeleteTimer()
{
	remainTime = duration - (SDL_GetTicks() - createdTime);
	if (remainTime <= 0) this->clean();

	//m_currentFrame = ((duration - remainTime) / 100) % 4;
	m_currentFrame = (duration - remainTime) / (duration / 4);
}

void FXAnimation::clean()
{
	SDLGameObject::clean();
}