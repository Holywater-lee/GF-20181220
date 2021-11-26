#include "Game.h"
#include "WIDTHHEIGHT.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char* argv[])
{
	Game::Instance()->init("20181220_LEESUNGSOO_GWAJE_2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	Uint32 frameStart, frameTime;

	while (Game::Instance()->running())
	{
		frameStart = SDL_GetTicks();
		Game::Instance()->handleEvents();
		Game::Instance()->update();
		Game::Instance()->render();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}
	}

	Game::Instance()->clean();
	return 0;
}