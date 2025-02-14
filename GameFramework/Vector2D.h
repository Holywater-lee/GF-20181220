#pragma once
#include "SDL.h"
#include <iostream>

class Vector2D
{
public:
	Vector2D(float x, float y) : m_x(x), m_y(y) {}
	float getX() { return m_x; }
	float getY() { return m_y; }
	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	float length() { return sqrt(m_x * m_x + m_y * m_y); }
	// a에서 b까지의 길이의 제곱을 반환
	static float LengthSquare(Vector2D& a, Vector2D& b) { return (a.m_x - b.m_x) * (a.m_x - b.m_x) + (a.m_y - b.m_y) * (a.m_y - b.m_y); }

	Vector2D operator+(const Vector2D& v2) const;
	Vector2D operator-(const Vector2D& v2) const;
	Vector2D operator*(float scalar);
	Vector2D operator/(float scalar);

	Vector2D& operator*=(float scalar);
	Vector2D& operator/=(float scalar);

	void normalize();

	friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2);

	// Lerp로 부드러운 움직임 구현
	static Vector2D Lerp(Vector2D& vec1, Vector2D& vec2, float time);

private:
	float m_x;
	float m_y;
};