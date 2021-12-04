#include "Camera.h"

Camera* Camera::s_pInstance = nullptr;

void Camera::Update(SDLGameObject* obj)
{
	// 매개변수로 받은 obj의 중앙(센터)에서 왼쪽 위로 화면의 절반씩 간 값
	Vector2D targetPos(obj->GetPos().getX() + obj->GetWidth() / 2 - width / 2, obj->GetPos().getY() + obj->GetHeight() / 2 - height / 2);
	// 카메라 위치는 Lerp로 부드러운 움직임 연출
	position = Vector2D::Lerp(position, targetPos, cameraSpeed);

	// 카메라 가두기
	if (position.getX() < 0) position.setX(0);
	if (position.getY() < 0) position.setY(0);
	if (position.getX() + width > LEVEL_WIDTH) position.setX(LEVEL_WIDTH - width);
	if (position.getY() + height > LEVEL_HEIGHT) position.setY(LEVEL_HEIGHT - height);
}

// 매개변수가 있다면 카메라에 상대적인 값을 가져옴
int Camera::GetX(float pos)
{
	return pos - position.getX();
}

// 위와 마찬가지
int Camera::GetY(float pos)
{
	return pos - position.getY();
}

// 카메라를 x, y값 만큼 이동시킴
void Camera::AddForce(float x, float y)
{
	position.setX(position.getX() + x);
	position.setY(position.getY() + y);
}

void Camera::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}