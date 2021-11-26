#pragma once
#ifndef __Game__
#define __Game__

#include "SDL.h"
#include "GameObject.h"
#include <vector>

#include "Text.h"
#include "Player.h"

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
	void update(float deltaTime);
	bool running();
	void handleEvents();
	void clean();
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	void CreateGameObject(GameObject* obj) { m_gameObjects.push_back(obj); }
	void CreateBulletObject(GameObject* obj) { m_bullets.push_back(obj); }
	void CreateTileObject(GameObject* obj) { m_tileObjects.push_back(obj); }
	void CreateText(Text* text) { m_texts.push_back(text); }

	std::vector<GameObject*> GetGameObjects() { return m_gameObjects; }
	std::vector<GameObject*> GetBulletObjects() { return m_bullets; }
	std::vector<GameObject*> GetTileObjects() { return m_tileObjects; }
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
	void RefreshGameObjects(std::vector<GameObject*>& list, GameObject& remove);
	void DetectCollision();

	std::vector<GameObject*> m_backgrounds;
	std::vector<GameObject*> m_gameObjects;
	std::vector<GameObject*> m_bullets;
	std::vector<GameObject*> m_tileObjects;
	std::vector<Text*> m_texts;
	Text* scoreText;
};
typedef Game TheGame;

#endif