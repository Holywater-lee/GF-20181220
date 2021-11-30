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
	cout << "�ʱ�ȭ ��..." << endl;
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
			cout << "������ �Ǵ� ������ �ʱ�ȭ ����" << endl;
			return false;
		}
	}
	else
	{
		cout << "SDL �ʱ�ȭ ����" << endl;
		return false;
	}

	if (!Init_Everything()) return false;

	cout << "�ʱ�ȭ ����!" << endl;
	m_bRunning = true;
	return true;
}

bool Game::Init_Everything()
{
	if (!TheLoadFiles::Instance()->Load())
	{
		cout << "���� �ҷ����� ����" << endl;
		return false;
	}
	if (!InitTextures()) return false;
	if (!InitTexts()) return false;

	TheMap::Instance()->CreateMap(0, 0);
	playerObject = new Player(new LoaderParams(32 * 2, 32 * 16, 32, 60, "Player"));
	CreateGameObject(playerObject);

	return true;
}

void Game::RestartGame()
{
	for (auto& go : m_gameObjects)
	{
		go->clean();
	}
	for (auto& bullet : m_bullets)
	{
		bullet->clean();
	}
	for (auto& tile : m_tiles)
	{
		tile->clean();
	}
	for (auto& fx : m_FXs)
	{
		fx->clean();
	}
	for (auto& text : m_texts)
	{
		text->clean();
	}
	RefreshGameObjects();

	m_gameObjects.clear();
	m_bullets.clear();
	m_tiles.clear();
	m_FXs.clear();
	m_texts.clear();

	TheTextManager::Instance()->clean();
	TheScore::Instance()->SetScore(0);
	RefreshScore();

	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		TheTextureManager::Instance()->DestroyTex(TheLoadFiles::Instance()->GetLoadedTexMaps(i));
	}

	Init_Everything();
}

bool Game::InitTextures()
{
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		string tempTexStr = TheLoadFiles::Instance()->GetLoadedTextures(i);
		string tempTexMapStr = TheLoadFiles::Instance()->GetLoadedTexMaps(i);

		if (!TheTextureManager::Instance()->load(tempTexStr, tempTexMapStr, m_pRenderer))
		{
			cout << "�ҷ����� ����: " << TheLoadFiles::Instance()->GetLoadedTexMaps(i) << endl;
			return false;
		}
	}

	return true;
}

bool Game::InitTexts()
{
	if (!TheTextManager::Instance()->InitFont("Assets/Fonts/NanumSquareRoundL.ttf", 64)) return false;

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 19, 256, 32, L"�¿� ����Ű�� �̵�", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 20, 256, 32, L"�� ����Ű�� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13, 32 * 19, 256, 32, L"AŰ�� ���� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13 - 16, 32 * 20, 256 + 32, 32, L"�Ʒ� ����Ű�� ���� ��� ��ȯ", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 26, 32 * 17, 256, 32, L"���� �� ������ ���� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 14, 32 * 10, 256, 32, L"������ ��� ���� ����", false);

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"����: ");
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

	TheTextureManager::Instance()->drawBackground("Background", m_pRenderer);
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
			if (dynamic_cast<SDLGameObject*>(go)->GetTag() == "Player")
			{
				playerObject = nullptr;
			}
			go = nullptr;
			delete go;
			RemoveGameObject(m_gameObjects, *go);
		}
	}
	for (auto& bullet : m_bullets)
	{
		if (!dynamic_cast<SDLGameObject*>(bullet)->GetIsActive())
		{
			bullet = nullptr;
			delete bullet;
			RemoveGameObject(m_bullets, *bullet);
		}
	}
	for (auto& tile : m_tiles)
	{
		if (!dynamic_cast<SDLGameObject*>(tile)->GetIsActive())
		{
			tile = nullptr;
			delete tile;
			RemoveGameObject(m_tiles, *tile);
		}
	}
	for (auto& fx : m_FXs)
	{
		if (!dynamic_cast<SDLGameObject*>(fx)->GetIsActive())
		{
			fx = nullptr;
			delete fx;
			RemoveGameObject(m_FXs, *fx);
		}
	}
	for (auto& text : m_texts)
	{
		if (!text->GetIsActive())
		{
			text = nullptr;
			delete text;
			m_texts.erase(std::remove(std::begin(m_texts), std::end(m_texts), text), std::end(m_texts));
		}
	}
}

int Game::GetRandomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 mersenne(rd()); // �޸��� Ʈ�����Ͷ�� ����� ����� rd�κ��� �õ带 ��´�
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