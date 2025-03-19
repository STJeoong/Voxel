#include "Vec2.h"
#include <utility>
#include <cmath>

#pragma region public static
float Vec2::dot(const Vec2& v1, const Vec2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
#pragma endregion


#pragma region public
float Vec2::len() const
{
	return std::sqrtf(x * x + y * y);
}
void Vec2::normalize()
{
	float length = this->len();
	if (length == 0.0f)
		return;
	x /= length;
	y /= length;
}
Vec2 Vec2::normalized() const
{
	Vec2 ret = *this;
	ret.normalize();
	return ret;
}
Vec2 Vec2::operator*(float val) const
{
	return { x * val, y * val };
}
Vec2 Vec2::operator+(const Vec2& other) const
{
	return { x + other.x, y + other.y };
}
Vec2 Vec2::operator-(const Vec2& other) const
{
	return { x - other.x, y - other.y };
}
Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
bool Vec2::operator==(const Vec2& other) const
{
	static const float TOLERANCE = 0.0001f;
	if (std::abs(x - other.x) <= TOLERANCE && std::abs(y - other.y) <= TOLERANCE)
		return true;
	return false;
}
#pragma endregion