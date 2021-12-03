#pragma once
#include "SDL.h"
#include "GameObject.h"
#include "Vector2D.h"
#include <string>

class SDLGameObject : public GameObject
{
public:
	SDLGameObject(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual Vector2D GetPos() const { return m_position; }
	virtual void OnHit();
	virtual bool GetIsActive() const { return isActive; }
	virtual ~SDLGameObject() {}

	std::string GetTag() const { return tag; }

	virtual int GetWidth() const { return m_width; }
	virtual int GetHeight() const { return m_height; }

protected:
	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;

	int m_width;
	int m_height;
	int m_currentRow;
	int m_currentFrame;
	std::string m_textureID;

	int life;

	// 활성화 되어있는지에 관한 변수, false일 경우 Game이 지움
	bool isActive = true;
	std::string tag = "";

	SDL_RendererFlip flip = SDL_FLIP_NONE;
};