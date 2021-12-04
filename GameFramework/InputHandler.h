#pragma once
#include "SDL.h"
#include "Vector2D.h"
#include <vector>

class InputHandler
{
public:
	~InputHandler() {}
	static InputHandler* Instance()
	{
		if (s_pInstance == 0)
			s_pInstance = new InputHandler();
		return s_pInstance;
	}
	void onKeyDown();
	void onKeyUp();
	bool isKeyDown(SDL_Scancode key);
	void update();
	void clean();

private:
	InputHandler();
	static InputHandler* s_pInstance;
	const Uint8* m_keystates;
};
typedef InputHandler TheInputHandler;