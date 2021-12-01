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
	virtual Vector2D GetPos() { return m_position; }
	virtual void OnHit();
	virtual bool GetIsActive() { return isActive; }
	virtual ~SDLGameObject() {}

	std::string GetTag() { return tag; }

	virtual int GetWidth() { return m_width; }
	virtual int GetHeight() { return m_height; }

protected:
	Vector2D m_position;
	Vector2D m_velocity;
	Vector2D m_acceleration;

	int m_width;
	int m_height;
	int m_currentRow;
	int m_currentFrame;
	int m_angle;
	std::string m_textureID;

	int life;

	bool isActive = true;
	std::string tag = "";

	SDL_RendererFlip flip = SDL_FLIP_NONE;
};