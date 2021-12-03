#include "InputHandler.h"
#include "Game.h"
#include "Camera.h"

InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler()
{
	m_mousePosition = new Vector2D(0, 0);
	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}
}

void InputHandler::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_KEYDOWN:
			onKeyDown();
			break;
		case SDL_KEYUP:
			onKeyUp();
			break;
			// ���콺 ������
		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;
			// ���콺 ����
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;
			// ���콺 ��
		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;
		default:
			break;
		}
	}
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keystates[key] == 1)
			return true;
		else
			return false;
	}

	return false;
}

bool InputHandler::getMouseButtonState(int buttonNumber)
{
	return m_mouseButtonStates[buttonNumber];
}

Vector2D* InputHandler::getMousePosition()
{
	return m_mousePosition;
}

// ī�޶� ��ġ�� ���� ������� ���콺 ��ġ�� �������� ����
void InputHandler::onMouseMove(SDL_Event event)
{
	m_mousePosition->setX(event.motion.x + TheCam::Instance()->GetX());
	m_mousePosition->setY(event.motion.y + TheCam::Instance()->GetY());
	/*
	int x, y;
	SDL_GetMouseState(&x, &y);
	x += TheCam::Instance()->GetX();
	y += TheCam::Instance()->GetY();
	m_mousePosition->setX(x);
	m_mousePosition->setY(y);
	*/
}

void InputHandler::onMouseButtonDown(SDL_Event event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = true;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = true;
	}
	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = true;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = false;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = false;
	}
	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = false;
	}
}

void InputHandler::onKeyDown()
{
	// SDL_GetKeyboardState: Uint8 �迭 (0~255)�� Ű������ ���� ���� ���¸� ����
	m_keystates = SDL_GetKeyboardState(0);
}
void InputHandler::onKeyUp()
{
	m_keystates = SDL_GetKeyboardState(0);
}

void InputHandler::clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}