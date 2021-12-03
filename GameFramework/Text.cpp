#include "Text.h"
#include "Game.h"
#include "Camera.h"

Text::Text(int x, int y, int width, int height, bool isUIText) : m_x(x), m_y(y), m_width(width), m_height(height), isUI(isUIText)
{
}

// �ؽ�Ʈ�� ��ġ�� �׷��� �Լ�
void Text::Draw()
{
	SDL_Rect textLocation;
	if (isUI) // ���� UI��� ȭ�鿡 ������Ų��
	{
		textLocation = { m_x, m_y, m_width, m_height };
	}
	else // UI�� �ƴ϶�� ȭ�鿡 ������Ű�� �ʰ� ī�޶� �����ӿ� ���� ��������� ��ġ�� ����ȴ�
	{
		textLocation = { TheCam::Instance()->GetX(m_x), TheCam::Instance()->GetY(m_y), m_width, m_height };
	}
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_texture, NULL, &textLocation);
}