#include "Vec3.h"
#include <utility>
#include <cmath>

#pragma region public static
float Vec3::dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2)
{
	Vec3 ret = {};
	ret.x = v1.y * v2.z - v1.z * v2.y;
	ret.y = v1.z * v2.x - v1.x * v2.z;
	ret.z = v1.x * v2.y - v1.y * v2.x;
	return ret;
}
#pragma endregion


#pragma region public
float Vec3::len() const
{
	return std::sqrtf(x * x + y * y + z * z);
}
void Vec3::normalize()
{
	float length = this->len();
	if (length == 0.0f)
		return;
	x /= length;
	y /= length;
	z /= length;
}
Vec3 Vec3::normalized() const
{
	Vec3 ret = *this;
	ret.normalize();
	return ret;
}
Vec3 Vec3::operator*(float val) const
{
	return { x * val, y * val, z * val };
}
Vec3 Vec3::operator+(const Vec3& other) const
{
	return { x + other.x, y + other.y, z + other.z };
}
Vec3 Vec3::operator-(const Vec3& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}
Vec3& Vec3::operator+=(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
bool Vec3::operator==(const Vec3& other) const
{
	static const float TOLERANCE = 0.0001f;
	if (std::abs(x - other.x) <= TOLERANCE && std::abs(y - other.y) <= TOLERANCE && std::abs(z - other.z) <= TOLERANCE)
		return true;
	return false;
}
#pragma endregion