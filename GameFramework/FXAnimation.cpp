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

// ���� �ð��� ����ϰ� ���� �ð��� �°� �������� �����Ű�� �Լ�, ���� �ð��� �� �Ǹ� clean ȣ��
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