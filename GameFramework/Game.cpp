#include "Game.h"
#include "InputHandler.h"

#include "Collision.h"

#include "TextManager.h"
#include "ScoreManager.h"
#include "TextureManager.h"
#include "MapManager.h"
#include "Audio.h"
#include "UIManager.h"
#include "LoadFiles.h"

#include "Player.h"

#include <algorithm>

#include "WIDTHHEIGHT.h"

Game* Game::s_pInstance = nullptr;

const SDL_Color color_white = { 255,255,255 };
const SDL_Color color_black = { 0,0,0 };
const SDL_Color color_blue = { 0,0,255 };
const SDL_Color color_red = { 255,0,0 };

// �ʱ�ȭ
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

	//vectorsList.emplace_back(move(m_tiles));
	//vectorsList.emplace_back(move(m_gameObjects));

	// ���� �ҷ����⸦ ����ϴ� �Լ�
	if (!TheLoadFiles::Instance()->Load())
	{
		cout << "���� �ҷ����� ����" << endl;
		return false;
	}

	// ����� �ʱ�ȭ
	if (!InitAudios()) return false;
	// �ؽ��� �ʱ�ȭ
	if (!InitTextures()) return false;
	// �ؽ�Ʈ ��Ʈ �ʱ�ȭ
	if (!TheTextManager::Instance()->InitFont("Assets/Fonts/NanumSquareRoundL.ttf", 64)) return false;

	// ��� ���� ��ü �ʱ�ȭ
	if (!Init_Everything()) return false;

	// ����� ���� ����
	TheAudio::Instance()->SetVolume(15);

	cout << "�ʱ�ȭ ����!" << endl;
	m_bRunning = true;
	return true;
}

// ��� ���� ��ü�� �ʱ�ȭ�ϴ� �Լ�
bool Game::Init_Everything()
{
	TheAudio::Instance()->PlayBGM();
	UIManager::Instance()->Init();

	if (!InitTexts()) return false;

	TheMap::Instance()->CreateMap(0, 0);
	playerObject = new Player(new LoaderParams(32 * 2, 32 * 16, 32, 60, "Player"));
	CreateGameObject(playerObject);

	return true;
}

// ������ ������ϴ� �Լ�
void Game::RestartGame()
{
	Clean_Everything(); // ��� ���� ��ü �����
	TheAudio::Instance()->StopBGM(); // BGM ����

	Init_Everything(); // ��� ���� ��ü �ʱ�ȭ
	TheScore::Instance()->SetScore(0); // ���� 0���� �ʱ�ȭ
	RefreshScore(); // ���� ����
}

// �ؽ��� �ʱ�ȭ
bool Game::InitTextures()
{
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		string tempTexStr = TheLoadFiles::Instance()->GetLoadedTextures(i);
		string tempTexMapStr = TheLoadFiles::Instance()->GetLoadedTexMaps(i);

		if (!TheTextureManager::Instance()->load(tempTexStr, tempTexMapStr))
		{
			cout << "�ҷ����� ����: " << tempTexMapStr << endl;
			return false;
		}
	}

	return true;
}

// �ؽ�Ʈ ��ü �ʱ�ȭ
bool Game::InitTexts()
{
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 19, 256, 32, L"�¿� ����Ű�� �̵�", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 20, 256, 32, L"�� ����Ű�� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13, 32 * 19, 256, 32, L"AŰ�� ���� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13 - 16, 32 * 20, 256 + 32, 32, L"�Ʒ� ����Ű�� ���� ��� ��ȯ", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 26, 32 * 17, 256, 32, L"���� �� ������ ���� ����", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 14, 32 * 10, 256, 32, L"������ ��� ���� ����", false);

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"����: ");
	scoreText_Score = TheTextManager::Instance()->LoadIntToText(color_white, color_black, SCREEN_WIDTH - 64, 0, 64, 32, TheScore::Instance()->GetScore());
	CreateText(scoreText_Score);

	return true;
}

// ����� �ʱ�ȭ �Լ�
bool Game::InitAudios()
{
	if (!TheAudio::Instance()->Init()) return false;
	TheAudio::Instance()->LoadBGM(TheLoadFiles::Instance()->GetLoadedBGM().c_str());

	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		string tempSfxStr = TheLoadFiles::Instance()->GetLoadedSfxFiles(i);
		string tempSfxMapStr = TheLoadFiles::Instance()->GetLoadedSfxMaps(i);

		if (!TheAudio::Instance()->LoadSFX(tempSfxStr.c_str(), tempSfxMapStr))
		{
			cout << "�ҷ����� ����: " << tempSfxMapStr << endl;
			return false;
		}
	}

	return true;
}

// ����(scoreText_Score)�� �����ϴ� �Լ�
void Game::RefreshScore()
{
	TheTextManager::Instance()->RefreshIntText(scoreText_Score, color_white, color_black, TheScore::Instance()->GetScore());
}

// �÷��̾� ��ġ�� �������� �Լ�
Vector2D Game::GetPlayerPos() const
{
	// �÷��̾ �׾��ٸ� (0, 0)�� ��ȯ
	if (dynamic_cast<Player*>(playerObject)->GetIsDead())
		return Vector2D(0, 0);

	if (playerObject != nullptr)
		return playerObject->GetPos();
	else return Vector2D(0, 0);
}

// �Ѿ��� �浹ó���� ����ϴ� �Լ�
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

	// ����� �׸��� �Լ�
	TheTextureManager::Instance()->drawBackground("Background");

	for (const auto& tile : m_tiles)
	{
		tile->draw();
	}
	for (const auto& go : m_gameObjects)
	{
		go->draw();
	}
	for (const auto& text : m_texts)
	{
		text->Draw();
	}

	// UI�� ���� ���� �׷���
	UIManager::Instance()->Draw();

	SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
	RefreshGameObjects();
	
	for (const auto& go : m_gameObjects)
	{
		go->update();
	}
	UIManager::Instance()->Update();

	DetectCollision();
}

// ���ӿ�����Ʈ �� isActive�� false�� ������Ʈ�� �����ִ� �Լ�
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

// ��� ������Ʈ�� �����ִ� �Լ�
void Game::Clean_Everything()
{
	int size_tile = m_tiles.size(); // for�� ���ο� ������ ��� ���ŵǾ���� �� ���ư��Ƿ� ������ �и�
	int size_go = m_gameObjects.size();
	int size_text = m_texts.size();

	// Ÿ�� ������Ʈ ����
	for (int i = 0; i < size_tile; i++)
	{
		// std::erase�� std::remove�� �̷���� RemoveGameObject�� ���� ������ ������ �� ĭ�� ������Ƿ� �׻� 0��°�� �˻�
		delete m_tiles[0];
		RemoveGameObject(m_tiles, *m_tiles[0]);
	}

	// ���ӿ�����Ʈ ����
	for (int i = 0; i < size_go; i++)
	{
		// �÷��̾��� ���߿� ���쵵��... �÷��̾ �����ϴ� ��� �÷��̾ ������� ����
		if (dynamic_cast<SDLGameObject*>(m_gameObjects[0])->GetTag() == "Player")
		{
			continue;
		}
		delete m_gameObjects[0];
		RemoveGameObject(m_gameObjects, *m_gameObjects[0]);
	}
	delete playerObject;
	RemoveGameObject(m_gameObjects, *playerObject);

	// �ؽ�Ʈ ������Ʈ ����
	for (int i = 0; i < size_text; i++)
	{
		delete m_texts[0];
		m_texts.erase(std::remove(std::begin(m_texts), std::end(m_texts), m_texts[0]), std::end(m_texts));
	}
	playerObject = nullptr;
	scoreText_Score = nullptr;
}

// ���ӿ�����Ʈ ���Ϳ��� Ư�� ���ӿ�����Ʈ�� �����ϰ� ���͸� �� ĭ�� ���� �� ĭ�� ���� �Լ�
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

// ����� �����ִ� �Լ�
void Game::clean()
{
	// ��� ������Ʈ�� ����
	Clean_Everything();
	// �ؽ��� ����
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		TheTextureManager::Instance()->DestroyTex(TheLoadFiles::Instance()->GetLoadedTexMaps(i));
	}
	// �����(ȿ����) ����
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		TheAudio::Instance()->RemoveSFX(TheLoadFiles::Instance()->GetLoadedSfxMaps(i));
	}
	// �����(�����) ����
	TheAudio::Instance()->RemoveBGM();

	// ���� �Ŵ��� �̱��� ����
	TheUI::Instance()->Clean();
	TheTextManager::Instance()->clean();
	TheAudio::Instance()->Clean();
	TheMap::Instance()->Clean();
	TheScore::Instance()->Clean();
	TheLoadFiles::Instance()->Clean();
	TheInputHandler::Instance()->clean();

	// ������ �� ������ ����
	if (m_pWindow != nullptr)
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
	SDL_Quit();
}