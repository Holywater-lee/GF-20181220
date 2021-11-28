#include "Camera.h"

Camera* Camera::s_pInstance = nullptr;

void Camera::Update(SDLGameObject* obj)
{
	Vector2D targetPos(obj->GetPos().getX() + obj->GetWidth() / 2 - SCREEN_WIDTH / 2, obj->GetPos().getY() + obj->GetHeight() / 2 - SCREEN_HEIGHT / 2);
	position = position.Lerp(position, targetPos, cameraSpeed);

	if (position.getX() < 0) position.setX(0);
	if (position.getY() < 0) position.setY(0);
	if (position.getX() + width > LEVEL_WIDTH) position.setX(LEVEL_WIDTH - width);
	if (position.getY() + height > LEVEL_HEIGHT) position.setY(LEVEL_HEIGHT - height);
}

int Camera::GetX(float pos)
{
	return pos - position.getX();
}

int Camera::GetY(float pos)
{
	return pos - position.getY();
}

void Camera::AddForce(float x, float y)
{
	position.setX(position.getX() + x);
	position.setY(position.getY() + y);
}