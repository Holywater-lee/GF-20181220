#include "Game.h"
#include "WIDTHHEIGHT.h"

/*
20181220 이성수
클래스의 각 함수에 관한 설명들은 .cpp 파일에 써 놓았습니다.
.cpp에 설명되지 않은 중요 내용들은 헤더 파일에 써 놓았습니다.
*/

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