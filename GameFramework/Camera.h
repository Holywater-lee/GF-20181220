#pragma once
#include "SDLGameObject.h"
#include "WIDTHHEIGHT.h"
#include "Vector2D.h"

class Camera
{
public:
	static Camera* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new Camera();
		return s_pInstance;
	}
	~Camera() {}

	void Update(SDLGameObject* obj);
	int GetX() { return position.getX(); }
	int GetY() { return position.getY(); }
	int GetX(float pos);
	int GetY(float pos);
	void AddForce(float x, float y);
	void Clean();
private:
	Camera() : position(0, 0) {}
	static Camera* s_pInstance;

	Vector2D position;

	const int width = SCREEN_WIDTH;
	const int height = SCREEN_HEIGHT;
	float cameraSpeed = 0.05f;
};
typedef Camera TheCam;