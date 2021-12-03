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

// 초기화
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

	// 파일 불러오기를 담당하는 함수
	if (!TheLoadFiles::Instance()->Load())
	{
		cout << "파일 불러오기 실패" << endl;
		return false;
	}

	// 오디오 초기화
	if (!InitAudios()) return false;
	// 텍스쳐 초기화
	if (!InitTextures()) return false;
	// 텍스트 폰트 초기화
	if (!TheTextManager::Instance()->InitFont("Assets/Fonts/NanumSquareRoundL.ttf", 64)) return false;

	// 모든 게임 객체 초기화
	if (!Init_Everything()) return false;

	// 오디오 볼륨 조절
	TheAudio::Instance()->SetVolume(15);

	cout << "초기화 성공!" << endl;
	m_bRunning = true;
	return true;
}

// 모든 게임 객체를 초기화하는 함수
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

// 게임을 재시작하는 함수
void Game::RestartGame()
{
	Clean_Everything(); // 모든 게임 객체 지우기
	TheAudio::Instance()->StopBGM(); // BGM 중지

	Init_Everything(); // 모든 게임 객체 초기화
	TheScore::Instance()->SetScore(0); // 점수 0으로 초기화
	RefreshScore(); // 점수 갱신
}

// 텍스쳐 초기화
bool Game::InitTextures()
{
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		string tempTexStr = TheLoadFiles::Instance()->GetLoadedTextures(i);
		string tempTexMapStr = TheLoadFiles::Instance()->GetLoadedTexMaps(i);

		if (!TheTextureManager::Instance()->load(tempTexStr, tempTexMapStr))
		{
			cout << "불러오기 실패: " << tempTexMapStr << endl;
			return false;
		}
	}

	return true;
}

// 텍스트 객체 초기화
bool Game::InitTexts()
{
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 19, 256, 32, L"좌우 방향키로 이동", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 2, 32 * 20, 256, 32, L"위 방향키로 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13, 32 * 19, 256, 32, L"A키를 눌러 공격", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 13 - 16, 32 * 20, 256 + 32, 32, L"아래 방향키로 공격 방식 전환", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 26, 32 * 17, 256, 32, L"점프 두 번으로 더블 점프", false);
	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, 32 * 14, 32 * 10, 256, 32, L"간격이 길게 더블 점프", false);

	TheTextManager::Instance()->LoadHanguelText(color_white, color_black, SCREEN_WIDTH - 128, 0, 64, 32, L"점수: ");
	scoreText_Score = TheTextManager::Instance()->LoadIntToText(color_white, color_black, SCREEN_WIDTH - 64, 0, 64, 32, TheScore::Instance()->GetScore());
	CreateText(scoreText_Score);

	return true;
}

// 오디오 초기화 함수
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
			cout << "불러오기 실패: " << tempSfxMapStr << endl;
			return false;
		}
	}

	return true;
}

// 점수(scoreText_Score)를 갱신하는 함수
void Game::RefreshScore()
{
	TheTextManager::Instance()->RefreshIntText(scoreText_Score, color_white, color_black, TheScore::Instance()->GetScore());
}

// 플레이어 위치를 가져오는 함수
Vector2D Game::GetPlayerPos() const
{
	// 플레이어가 죽었다면 (0, 0)을 반환
	if (dynamic_cast<Player*>(playerObject)->GetIsDead())
		return Vector2D(0, 0);

	if (playerObject != nullptr)
		return playerObject->GetPos();
	else return Vector2D(0, 0);
}

// 총알의 충돌처리를 담당하는 함수
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

	// 배경을 그리는 함수
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

	// UI가 가장 위에 그려짐
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

// 게임오브젝트 중 isActive가 false인 오브젝트를 지워주는 함수
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

// 모든 오브젝트를 지워주는 함수
void Game::Clean_Everything()
{
	int size_tile = m_tiles.size(); // for문 내부에 넣으면 계속 갱신되어버려 덜 돌아가므로 별개로 분리
	int size_go = m_gameObjects.size();
	int size_text = m_texts.size();

	// 타일 오브젝트 제거
	for (int i = 0; i < size_tile; i++)
	{
		// std::erase와 std::remove로 이루어진 RemoveGameObject에 의해 삭제될 때마다 한 칸씩 당겨지므로 항상 0번째를 검사
		delete m_tiles[0];
		RemoveGameObject(m_tiles, *m_tiles[0]);
	}

	// 게임오브젝트 제거
	for (int i = 0; i < size_go; i++)
	{
		// 플레이어라면 나중에 지우도록... 플레이어를 참조하는 경우 플레이어가 사라지면 에러
		if (dynamic_cast<SDLGameObject*>(m_gameObjects[0])->GetTag() == "Player")
		{
			continue;
		}
		delete m_gameObjects[0];
		RemoveGameObject(m_gameObjects, *m_gameObjects[0]);
	}
	delete playerObject;
	RemoveGameObject(m_gameObjects, *playerObject);

	// 텍스트 오브젝트 제거
	for (int i = 0; i < size_text; i++)
	{
		delete m_texts[0];
		m_texts.erase(std::remove(std::begin(m_texts), std::end(m_texts), m_texts[0]), std::end(m_texts));
	}
	playerObject = nullptr;
	scoreText_Score = nullptr;
}

// 게임오브젝트 벡터에서 특정 게임오브젝트를 삭제하고 벡터를 빈 칸이 없게 한 칸씩 당기는 함수
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

// 지울거 지워주는 함수
void Game::clean()
{
	// 모든 오브젝트를 지움
	Clean_Everything();
	// 텍스쳐 삭제
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetTexMapsSize(); i++)
	{
		TheTextureManager::Instance()->DestroyTex(TheLoadFiles::Instance()->GetLoadedTexMaps(i));
	}
	// 오디오(효과음) 삭제
	for (size_t i = 0; i < TheLoadFiles::Instance()->GetSfxMapsSize(); i++)
	{
		TheAudio::Instance()->RemoveSFX(TheLoadFiles::Instance()->GetLoadedSfxMaps(i));
	}
	// 오디오(배경음) 삭제
	TheAudio::Instance()->RemoveBGM();

	// 각종 매니저 싱글톤 삭제
	TheUI::Instance()->Clean();
	TheTextManager::Instance()->clean();
	TheAudio::Instance()->Clean();
	TheMap::Instance()->Clean();
	TheScore::Instance()->Clean();
	TheLoadFiles::Instance()->Clean();
	TheInputHandler::Instance()->clean();

	// 윈도우 및 렌더러 삭제
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