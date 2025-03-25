#include "Vec3.h"
#include <utility>
#include <cmath>
#include "Mat4.h"

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
Vec3::Vec3(const Vec3& other) : x(_v[0]), y(_v[1]), z(_v[2])
{
	for (int i = 0; i < 3; ++i)
		_v[i] = other._v[i];
}
Vec3& Vec3::operator=(const Vec3& other)
{
	for (int i = 0; i < 3; ++i)
		_v[i] = other._v[i];
	return *this;
}
Vec3::Vec3(float x, float y, float z) : x(_v[0]), y(_v[1]), z(_v[2])
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
}
Vec3::Vec3(float v[3]) : x(_v[0]), y(_v[1]), z(_v[2])
{
	for (int i = 0; i < 3; ++i)
		_v[i] = v[i];
}
float Vec3::len() const
{
	return std::sqrt(x * x + y * y + z * z);
}
void Vec3::normalize()
{
	float length = this->len();
	if (length == 0.0f)
		return;
	for (int i = 0; i < 3; ++i)
		_v[i] /= length;
}
Vec3 Vec3::normalized() const
{
	Vec3 ret(*this);
	ret.normalize();
	return ret;
}
Mat4 Vec3::asScale() const
{
	float m[4][4] = {};
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	m[3][3] = 1.0f;
	return Mat4(m);
}
Mat4 Vec3::asTranslation() const
{
	float m[4][4] = {};
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;
	return Mat4(m);
}
Vec3 Vec3::operator*(float val) const
{
	Vec3 ret(*this);
	for (int i = 0; i < 3; ++i)
		ret._v[i] *= val;
	return ret;
}
Vec3 Vec3::operator+(const Vec3& other) const
{
	Vec3 ret(*this);
	for (int i = 0; i < 3; ++i)
		ret._v[i] += other._v[i];
	return ret;
}
Vec3 Vec3::operator-(const Vec3& other) const
{
	Vec3 ret(*this);
	for (int i = 0; i < 3; ++i)
		ret._v[i] -= other._v[i];
	return ret;
}
Vec3& Vec3::operator*=(float val)
{
	for (int i = 0; i < 3; ++i)
		_v[i] *= val;
	return *this;
}
Vec3& Vec3::operator+=(const Vec3& other)
{
	for (int i = 0; i < 3; ++i)
		_v[i] += other._v[i];
	return *this;
}
Vec3& Vec3::operator-=(const Vec3& other)
{
	for (int i = 0; i < 3; ++i)
		_v[i] -= other._v[i];
	return *this;
}
bool Vec3::operator==(const Vec3& other) const
{
	static const float TOLERANCE = 0.0001f;
	for (int i = 0; i < 3; ++i)
		if (std::abs(_v[i] - other._v[i]) > TOLERANCE)
			return false;
	return true;
}
#pragma endregion



Vec3 operator*(float val, const Vec3& vec)
{
	Vec3 ret(vec);
	for (int i = 0; i < 3; ++i)
		ret._v[i] *= val;
	return ret;
}
const Vec3 Vec3::X = { 1.0f, 0.0f, 0.0f };
const Vec3 Vec3::Y = { 0.0f, 1.0f, 0.0f };
const Vec3 Vec3::Z = { 0.0f, 0.0f, 1.0f };