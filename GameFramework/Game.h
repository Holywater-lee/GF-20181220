#pragma once
#ifndef __Game__
#define __Game__

#include "SDL.h"
#include "GameObject.h"
#include "Vector2D.h"

#include "Text.h"
#include <vector>
#include <list>

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
	
	void RestartGame();
	void quit() { m_bRunning = false; }
	void render();
	void update();
	bool running();
	void handleEvents();
	void clean();
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	void CreateText(Text* text) { m_texts.push_back(text); }
	
	void CreateTileObject(GameObject* obj) { m_tiles.push_back(obj); }
	void CreateGameObject(GameObject* obj) { m_gameObjects.push_back(obj); }

	vector<GameObject*> GetTileObjects() { return m_tiles; }
	vector<GameObject*> GetGameObjects() { return m_gameObjects; }

	void RefreshScore();

	Vector2D GetPlayerPos() const;

private:
	Game() {}
	static Game* s_pInstance;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	bool m_bRunning;

	bool InitTextures();
	bool InitTexts();
	bool InitAudios();
	bool Init_Everything();
	void Clean_Everything();

	void RefreshGameObjects();
	void RemoveGameObject(vector<GameObject*>& list, GameObject& remove);
	void DetectCollision();

	vector<GameObject*> m_tiles;
	vector<GameObject*> m_gameObjects;
	//list<vector<GameObject*>> vectorsList;

	vector<Text*> m_texts;
	GameObject* playerObject;
	Text* scoreText_Score;
};
typedef Game TheGame;

#endif