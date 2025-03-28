#include "Vec4.h"
#include <cmath>
#include "Vec3.h"

#pragma region public
Vec4::Vec4(const Vec4& other) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3])
{
	for (int i = 0; i < 4; ++i)
		_v[i] = other._v[i];
}
Vec4& Vec4::operator=(const Vec4& other)
{
	for (int i = 0; i < 4; ++i)
		_v[i] = other._v[i];
	return *this;
}
Vec4::Vec4(float x, float y, float z, float w) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3])
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
	_v[3] = w;
}
Vec4::Vec4(float v[4]) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3])
{
	for (int i = 0; i < 4; ++i)
		_v[i] = v[i];
}
Vec4::Vec4(const Vec3& vec3, float w) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3])
{
	for (int i = 0; i < 3; ++i)
		_v[i] = vec3._v[i];
	_v[3] = w;
}
float Vec4::len() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}
float Vec4::squaredLen() const
{
	return x * x + y * y + z * z + w * w;
}
void Vec4::normalize()
{
	float length = this->len();
	if (length == 0.0f)
		return;
	for (int i = 0; i < 4; ++i)
		_v[i] /= length;
}
Vec4 Vec4::normalized() const
{
	Vec4 ret = *this;
	ret.normalize();
	return ret;
}
Vec4 Vec4::operator*(float val) const
{
	Vec4 ret(*this);
	for (int i = 0; i < 4; ++i)
		ret._v[i] *= val;
	return ret;
}
Vec4 Vec4::operator+(const Vec4& other) const
{
	Vec4 ret(*this);
	for (int i = 0; i < 4; ++i)
		ret._v[i] += other._v[i];
	return ret;
}
Vec4 Vec4::operator-(const Vec4& other) const
{
	Vec4 ret(*this);
	for (int i = 0; i < 4; ++i)
		ret._v[i] -= other._v[i];
	return ret;
}
Vec4& Vec4::operator+=(const Vec4& other)
{
	for (int i = 0; i < 4; ++i)
		_v[i] += other._v[i];
	return *this;
}
Vec4& Vec4::operator-=(const Vec4& other)
{
	for (int i = 0; i < 4; ++i)
		_v[i] -= other._v[i];
	return *this;
}
bool Vec4::operator==(const Vec4& other) const
{
	static const float TOLERANCE = 0.0001f;
	for (int i = 0; i < 4; ++i)
		if (std::abs(_v[i] - other._v[i]) > TOLERANCE)
			return false;
	return true;
}
#pragma endregion