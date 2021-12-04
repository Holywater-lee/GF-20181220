#include "InputHandler.h"
#include "Game.h"
#include "Camera.h"

InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler()
{
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

void InputHandler::onKeyDown()
{
	// SDL_GetKeyboardState: Uint8 배열 (0~255)에 키보드의 현재 눌린 상태를 저장
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