#include "Camera.h"

Camera* Camera::s_pInstance = nullptr;

void Camera::Update(SDLGameObject* obj)
{
	// �Ű������� ���� obj�� �߾�(����)���� ���� ���� ȭ���� ���ݾ� �� ��
	Vector2D targetPos(obj->GetPos().getX() + obj->GetWidth() / 2 - width / 2, obj->GetPos().getY() + obj->GetHeight() / 2 - height / 2);
	// ī�޶� ��ġ�� Lerp�� �ε巯�� ������ ����
	position = Vector2D::Lerp(position, targetPos, cameraSpeed);

	// ī�޶� ���α�
	if (position.getX() < 0) position.setX(0);
	if (position.getY() < 0) position.setY(0);
	if (position.getX() + width > LEVEL_WIDTH) position.setX(LEVEL_WIDTH - width);
	if (position.getY() + height > LEVEL_HEIGHT) position.setY(LEVEL_HEIGHT - height);
}

// �Ű������� �ִٸ� ī�޶� ������� ���� ������
int Camera::GetX(float pos)
{
	return pos - position.getX();
}

// ���� ��������
int Camera::GetY(float pos)
{
	return pos - position.getY();
}

// ī�޶� x, y�� ��ŭ �̵���Ŵ
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