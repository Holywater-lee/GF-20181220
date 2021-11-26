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
	std::cout << "초기화 중..." << std::endl;
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
			std::cout << "윈도우 또는 렌더러 초기화 실패" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "SDL 초기화 실패" << std::endl;
		return false;
	}

	if (!TheLoadFiles::Instance()->Load())
	{
		std::cout << "파일 불러오기 실패" << std::endl;
		return false;
	}
	if (!InitTextures()) return false;
	if (!InitTexts()) return false;

	InitBackgrounds();
	TheMap::Instance()->CreateMap(0, 0);

	CreateGameObject(new Player(new LoaderParams(128, 32, 32, 32, "Player")));

	std::cout << "초기화 성공!" << std::endl;
	m_bRunning = true;
	return true;
}

bool Game::InitTextures()
{
	/*
	if (!TheTextureManager::Instance()->load("Assets/Player-Sheet.png", "Player", m_pRenderer)) return false;
	if (!TheTextureManager::Instance()->load("Assets/Ball-Sheet.png", "Ball", m_pRenderer)) return false;
	if (!TheTextureManager::Instance()->load("Assets/Enemy-Sheet.png", "Enemy", m_pRenderer)) return false;
	if (!TheTextureManager::Instance()->load("Assets/Tile-stone.png", "Tile", m_pRenderer)) return false;
	if (!TheTextureManager::Instance()->load("Assets/back.png", "Background", m_pRenderer)) return false;

	return true;
	*/

	// 굉장히 어이없는 버그.... 텍스트를 다시 작성하니 바로 해결...... 몇시간을썻는데,,,,,ㅜㅜ
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		std::string tempTexStr = TheLoadFiles::Instance()->GetLoadedTextures(i);
		std::string tempTexMapStr = TheLoadFiles::Instance()->GetLoadedTexMaps(i);

		if (!TheTextureManager::Instance()->load(tempTexStr, tempTexMapStr, m_pRenderer))
		{
			std::cout << "불러오기 실패: " << TheLoadFiles::Instance()->GetLoadedTexMaps(i) << std::endl;
			return false;
		}
	}

	return true;
}

bool Game::InitTexts()
{
	if (!TheTextManager::Instance()->InitFont("Assets/Fonts/NanumSquareRoundL.ttf", 64)) return false;

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"점수: ");
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 256, 32, 256, 32, L"공격: 마우스 클릭");
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

		for (const auto& tile : m_tileObjects)
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
	for (const auto& tile : m_tileObjects)
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
	for (const auto& text : m_texts)
	{
		text->Draw();
	}

	SDL_RenderPresent(m_pRenderer);
}

void Game::update(float deltaTime)
{
	RefreshGameObjects();

	for (const auto& bgs : m_backgrounds)
	{
		bgs->update(deltaTime);
	}
	for (const auto& go : m_gameObjects)
	{
		go->update(deltaTime);
	}
	for (const auto& bullet : m_bullets)
	{
		bullet->update(deltaTime);
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
			RefreshGameObjects(m_gameObjects, *go);
		}
	}
	for (auto& bullet : m_bullets)
	{
		if (!dynamic_cast<SDLGameObject*>(bullet)->GetIsActive())
		{
			delete bullet;
			RefreshGameObjects(m_bullets, *bullet);
		}
	}
}

void Game::RefreshGameObjects(std::vector<GameObject*>& list, GameObject& remove)
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