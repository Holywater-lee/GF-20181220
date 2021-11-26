#include "Game.h"
#include "InputHandler.h"

#include "Collision.h"

#include "TextManager.h"
#include "ScoreManager.h"
#include "TextureManager.h"
#include "LoadFiles.h"
#include "MapManager.h"

#include "ScrollingBackGround.h"

#include <algorithm>

#include "WIDTHHEIGHT.h"

Game* Game::s_pInstance = 0;

const SDL_Color color_white = { 255,255,255 };
const SDL_Color color_black = { 0,0,0 };
const SDL_Color color_blue = { 0,0,255 };
const SDL_Color color_red = { 255,0,0 };

bool Game::init(const char* title, int xpos, int ypos, int height, int width, int flags)
{
	cout << "초기화 중..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);

		if (m_pWindow != 0)
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0)
			{
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
		}

		if (m_pWindow == 0 || m_pRenderer == 0)
		{
			cout << "윈도우 또는 렌더러 초기화 실패" << endl;
			return false;
		}
	}
	else
	{
		cout << "SDL 초기화 실패" << endl;
		return false;
	}

	if (!TheLoadFiles::Instance()->Load())
	{
		cout << "파일 불러오기 실패" << endl;
		return false;
	}
	if (!InitTextures()) return false;
	if (!InitTexts()) return false;

	InitBackgrounds();
	TheMap::Instance()->CreateMap(0, 0);
	CreateGameObject(new Player(new LoaderParams(32*2, 32*16, 32, 60, "Player")));

	cout << "초기화 성공!" << endl;
	m_bRunning = true;
	return true;
}

bool Game::InitTextures()
{
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		string tempTexStr = TheLoadFiles::Instance()->GetLoadedTextures(i);
		string tempTexMapStr = TheLoadFiles::Instance()->GetLoadedTexMaps(i);

		if (!TheTextureManager::Instance()->load(tempTexStr, tempTexMapStr, m_pRenderer))
		{
			cout << "불러오기 실패: " << TheLoadFiles::Instance()->GetLoadedTexMaps(i) << endl;
			return false;
		}
	}

	return true;
}

bool Game::InitTexts()
{
	if (!TheTextManager::Instance()->InitFont("Assets/Fonts/NanumSquareRoundL.ttf", 64)) return false;

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"점수: ");
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 19, 256, 32, L"좌우 방향키로 이동", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 20, 256, 32, L"위 방향키로 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13 - 16, 32 * 19, 256 + 32, 32, L"마우스 왼쪽 클릭으로 원거리 공격", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13, 32 * 20, 256, 32, L"아래 방향키로 근접 공격", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 26, 32 * 17, 256, 32, L"점프 두 번으로 더블 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 14, 32 * 10, 256, 32, L"간격이 길게 더블 점프", false);
	scoreText = TheTextManager::Instance()->LoadIntToText(color_white, color_black, SCREEN_WIDTH - 64, 0, 64, 32, TheScore::Instance()->GetScore());
	CreateText(scoreText);

	return true;
}

void Game::InitBackgrounds()
{
	m_backgrounds.push_back(new ScrollingBackGround(new LoaderParams(0, 0, 1280, 720, "Background"), 1));
	m_backgrounds.push_back(new ScrollingBackGround(new LoaderParams(1280, 0, 1280, 720, "Background"), 1));
}

void Game::RefreshScore()
{
	TheTextManager::Instance()->RefreshIntText(scoreText, color_white, color_black, TheScore::Instance()->GetScore());
}

void Game::DetectCollision()
{
	for (auto& bullet : m_bullets)
	{
		for (const auto& go : m_gameObjects)
		{
			if (Collision::onCollision(bullet, go))
			{
				if (dynamic_cast<SDLGameObject*>(go)->GetTag() == "Enemy")
				{
					bullet->clean();
					go->OnHit();
				}
			}
		}

		for (const auto& tile : m_tiles)
		{
			if (Collision::onCollision(bullet, tile))
			{
				bullet->clean();
			}
		}
	}
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	for (const auto& bgs : m_backgrounds)
	{
		bgs->draw();
	}
	for (const auto& tile : m_tiles)
	{
		tile->draw();
	}
	for (const auto& go : m_gameObjects)
	{
		go->draw();
	}
	for (const auto& bullet : m_bullets)
	{
		bullet->draw();
	}
	for (const auto& fx : m_FXs)
	{
		fx->draw();
	}
	for (const auto& text : m_texts)
	{
		text->Draw();
	}

	SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
	RefreshGameObjects();

	for (const auto& bgs : m_backgrounds)
	{
		bgs->update();
	}
	for (const auto& go : m_gameObjects)
	{
		go->update();
	}
	for (const auto& bullet : m_bullets)
	{
		bullet->update();
	}
	for (const auto& fx : m_FXs)
	{
		fx->update();
	}

	DetectCollision();
}

void Game::RefreshGameObjects()
{
	for (auto& go : m_gameObjects)
	{
		if (!dynamic_cast<SDLGameObject*>(go)->GetIsActive())
		{
			delete go;
			RemoveGameObject(m_gameObjects, *go);
		}
	}
	for (auto& bullet : m_bullets)
	{
		if (!dynamic_cast<SDLGameObject*>(bullet)->GetIsActive())
		{
			delete bullet;
			RemoveGameObject(m_bullets, *bullet);
		}
	}
	for (auto& fx : m_FXs)
	{
		if (!dynamic_cast<SDLGameObject*>(fx)->GetIsActive())
		{
			delete fx;
			RemoveGameObject(m_FXs, *fx);
		}
	}
}

void Game::RemoveGameObject(vector<GameObject*>& list, GameObject& remove)
{
	list.erase(std::remove(std::begin(list), std::end(list), &remove), std::end(list));
}

bool Game::running()
{
	return m_bRunning;
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}

void Game::clean()
{
	TheTextManager::Instance()->clean();

	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		TheTextureManager::Instance()->DestroyTex(TheLoadFiles::Instance()->GetLoadedTexMaps(i));
	}

	if (m_pWindow != 0) SDL_DestroyWindow(m_pWindow);
	if (m_pRenderer != 0) SDL_DestroyRenderer(m_pRenderer);
	TheInputHandler::Instance()->clean();
	SDL_Quit();
}