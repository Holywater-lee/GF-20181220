#include "Vector2D.h"

Vector2D Vector2D::operator+(const Vector2D& v2) const
{
	return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
}

Vector2D Vector2D::operator-(const Vector2D& v2) const
{
	return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
}

Vector2D Vector2D::operator*(float scalar)
{
	return Vector2D(m_x * scalar, m_y * scalar);
}

Vector2D Vector2D::operator/(float scalar)
{
	return Vector2D(m_x / scalar, m_y / scalar);
}

Vector2D& Vector2D::operator*=(float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	return *this;
}

Vector2D& Vector2D::operator/=(float scalar)
{
	m_x /= scalar;
	m_y /= scalar;
	return *this;
}

void Vector2D::normalize()
{
	float l = length();
	if (l > 0)
	{
		(*this) *= 1 / l;
	}
}

Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	return v1;
}

Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	return v1;
}

// Lerp 연산으로 부드러운 움직임 가능
Vector2D Vector2D::Lerp(Vector2D& vec1, Vector2D& vec2, float time)
{
	return (vec1 + (vec2 - vec1) * time);
}