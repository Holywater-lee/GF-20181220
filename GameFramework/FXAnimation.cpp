#include "FXAnimation.h"

FXAnimation::FXAnimation(const LoaderParams* pParams, int sdlgetticks, int p_duration, int startRow, int startFrame, bool flipped, int frame) : SDLGameObject(pParams), createdTime(sdlgetticks), duration(p_duration), startFrame(startFrame), totalFrame(frame)
{
	tag = "FX";
	flip = flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	remainTime = duration - (SDL_GetTicks() - createdTime);
	m_currentRow = startRow;
}

void FXAnimation::draw()
{
	SDLGameObject::draw();
}

void FXAnimation::update()
{
	DeleteTimer();
}

// 남은 시간을 계산하고 남은 시간에 맞게 프레임을 재생시키는 함수, 남은 시간이 다 되면 clean 호출
void FXAnimation::DeleteTimer()
{
	remainTime = duration - (SDL_GetTicks() - createdTime);
	if (remainTime <= 0) this->clean();

	if (totalFrame != 1)
		m_currentFrame = startFrame + (duration - remainTime) / (duration / totalFrame);
	else
		m_currentFrame = startFrame;
}

void FXAnimation::clean()
{
	SDLGameObject::clean();
}