#pragma once
#ifndef __Game__
#define __Game__

#include "SDL.h"
#include "GameObject.h"
#include <vector>

#include "Text.h"
#include "Player.h"

using namespace std;

class Game
{
public:
	static Game* Instance()
	{
		if (s_pInstance == 0) s_pInstance = new Game();
		return s_pInstance;
	}
	~Game() {}

	bool init(const char* title, int xpos, int ypos, int height, int width, int flags);
	void quit() { m_bRunning = false; }
	void render();
	void update();
	bool running();
	void handleEvents();
	void clean();
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	void CreateGameObject(GameObject* obj) { m_gameObjects.push_back(obj); }
	void CreateBulletObject(GameObject* obj) { m_bullets.push_back(obj); }
	void CreateTileObject(GameObject* obj) { m_tiles.push_back(obj); }
	void CreateFX(GameObject* obj) { m_FXs.push_back(obj); }
	void CreateText(Text* text) { m_texts.push_back(text); }

	vector<GameObject*> GetGameObjects() { return m_gameObjects; }
	vector<GameObject*> GetTileObjects() { return m_tiles; }
	void RefreshScore();

private:
	Game() {}
	static Game* s_pInstance;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	bool m_bRunning;

	bool InitTextures();
	bool InitTexts();
	void InitBackgrounds();

	void RefreshGameObjects();
	void RemoveGameObject(vector<GameObject*>& list, GameObject& remove);
	void DetectCollision();

	vector<GameObject*> m_backgrounds;
	vector<GameObject*> m_gameObjects;
	vector<GameObject*> m_bullets;
	vector<GameObject*> m_tiles;
	vector<GameObject*> m_FXs;

	vector<Text*> m_texts;
	Text* scoreText;
};
typedef Game TheGame;

#endif