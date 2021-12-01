#include "Game.h"
#include "InputHandler.h"

#include "Collision.h"

#include "TextManager.h"
#include "ScoreManager.h"
#include "TextureManager.h"
#include "LoadFiles.h"
#include "MapManager.h"

#include "Player.h"

#include <algorithm>
#include <random>

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

	//vectorsList.emplace_back(move(m_tiles));
	//vectorsList.emplace_back(move(m_gameObjects));

	if (!TheLoadFiles::Instance()->Load())
	{
		cout << "파일 불러오기 실패" << endl;
		return false;
	}

	if (!Init_Everything()) return false;

	cout << "초기화 성공!" << endl;
	m_bRunning = true;
	return true;
}

bool Game::Init_Everything()
{
	if (!InitTextures()) return false;
	if (!InitTexts()) return false;

	TheMap::Instance()->CreateMap(0, 0);
	playerObject = new Player(new LoaderParams(32 * 2, 32 * 16, 32, 60, "Player"));
	CreateGameObject(playerObject);

	return true;
}

void Game::RestartGame()
{
	Clean_Everything();

	TheTextManager::Instance()->clean();

	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		TheTextureManager::Instance()->DestroyTex(TheLoadFiles::Instance()->GetLoadedTexMaps(i));
	}

	Init_Everything();
	TheScore::Instance()->SetScore(0);
	RefreshScore();
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

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 19, 256, 32, L"좌우 방향키로 이동", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 20, 256, 32, L"위 방향키로 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13, 32 * 19, 256, 32, L"A키를 눌러 공격", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13 - 16, 32 * 20, 256 + 32, 32, L"아래 방향키로 공격 방식 전환", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 26, 32 * 17, 256, 32, L"점프 두 번으로 더블 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 14, 32 * 10, 256, 32, L"간격이 길게 더블 점프", false);

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"점수: ");
	scoreText = TheTextManager::Instance()->LoadIntToText(color_white, color_black, SCREEN_WIDTH - 64, 0, 64, 32, TheScore::Instance()->GetScore());
	CreateText(scoreText);

	return true;
}

void Game::RefreshScore()
{
	TheTextManager::Instance()->RefreshIntText(scoreText, color_white, color_black, TheScore::Instance()->GetScore());
}

Vector2D Game::GetPlayerPos() const
{
	if (playerObject != nullptr)
		return playerObject->GetPos();
	else
		return Vector2D(0, 0);
}

void Game::DetectCollision()
{
	for (auto& bullet : m_gameObjects)
	{
		if (dynamic_cast<SDLGameObject*>(bullet)->GetTag() == "Bullet")
		{
			for (const auto& go : m_gameObjects)
			{
				if (bullet != go && Collision::onCollision(bullet, go))
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
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	TheTextureManager::Instance()->drawBackground("Background", m_pRenderer);
	
	for (int i = 0; i < 2; i++)
	{
		vector<GameObject*> tempVector;
		switch (i)
		{
		case 0:
			tempVector = m_tiles;
			break;
		case 1:
			tempVector = m_gameObjects;
			break;
		default:
			break;
		}

		for (const auto& go : tempVector)
		{
			go->draw();
		}
	}
	/*
	list<vector<GameObject*>>::iterator listIter;
	vector<GameObject*>::iterator vecIter;
	for (listIter = vectorsList.begin(); listIter != vectorsList.end(); listIter++)
	{
		for (vecIter = listIter->begin(); vecIter != listIter->end(); vecIter++)
		{
			(*vecIter)->draw();
		}
	}*/
	for (const auto& text : m_texts)
	{
		text->Draw();
	}

	SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
	RefreshGameObjects();
	
	for (const auto& go : m_gameObjects)
	{
		go->update();
	}

	DetectCollision();
}

void Game::RefreshGameObjects()
{
	for (auto& go : m_gameObjects)
	{
		if (!go->GetIsActive())
		{
			delete go;
			RemoveGameObject(m_gameObjects, *go);
		}
	}
}

void Game::Clean_Everything()
{
	int size_tile = m_tiles.size(); // for문 내부에 넣으면 계속 갱신되어버려 덜 돌아감
	int size_go = m_gameObjects.size();
	int size_text = m_texts.size();

	for (int i = 0; i < size_tile; i++)
	{
		delete m_tiles[0];
		RemoveGameObject(m_tiles, *m_tiles[0]);
	}

	for (int i = 0; i < size_go; i++)
	{
		// std::erase와 std::remove로 이루어진 RemoveGameObject에 의해 삭제될 때마다 한 칸씩 당겨지므로
		// 항상 0번째를 검사
		if (dynamic_cast<SDLGameObject*>(m_gameObjects[0])->GetTag() == "Player")
		{
			continue;
		}
		delete m_gameObjects[0];
		RemoveGameObject(m_gameObjects, *m_gameObjects[0]);
	}
	delete playerObject;
	RemoveGameObject(m_gameObjects, *playerObject);

	for (int i = 0; i < size_text; i++)
	{
		delete m_texts[0];
		m_texts.erase(std::remove(std::begin(m_texts), std::end(m_texts), m_texts[0]), std::end(m_texts));
	}
	playerObject = nullptr;
	scoreText = nullptr;
}

int Game::GetRandomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 mersenne(rd()); // 메르센 트위스터라는 방법을 사용해 rd로부터 시드를 얻는다
	std::uniform_int_distribution<> value(min, max);
	return value(mersenne);
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