#include "Quat.h"
#include <cmath>
#include "Mat4.h"

#pragma region public static
Quat Quat::slerp(const Quat& q1, const Quat& q2, float f)
{
	return Quat();
}
#pragma endregion


#pragma region public
Quat::Quat(const Vec3& axis, float radian)
{
	_s = cos(radian / 2.0f);
	_v = axis * sin(radian / 2.0f);
}
// yaw(y), pitch(x), roll(z) : 정면이 z축이고 위쪽이 y축 오른손 좌표
Quat::Quat(const Vec3& euler)
{
	float cr = cos(euler.x * 0.5f);
	float sr = sin(euler.x * 0.5f);
	float cp = cos(euler.y * 0.5f);
	float sp = sin(euler.y * 0.5f);
	float cy = cos(euler.z * 0.5f);
	float sy = sin(euler.z * 0.5f);

	_s = cr * cp * cy + sr * sp * sy;
	_v = { sr * cp * cy - cr * sp * sy , cr * sp * cy + sr * cp * sy, cr * cp * sy - sr * sp * cy };
}
Quat& Quat::operator=(const Vec3& euler)
{
	*this = Quat(euler);
	return *this;
}
Quat& Quat::operator=(const Quat& other)
{
	_s = other._s;
	_v = other._v;
	return *this;
}
Quat& Quat::operator*=(const Quat& other)
{
	*this = other * (*this);
	return *this;
}
Quat Quat::operator*(const Quat& other) const
{
	Quat ret;
	float w1 = _s, w2 = other._s;
	const Vec3& v1 = _v, v2 = other._v;
	ret._s = w1 * w2 - Vec3::dot(v1, v2);
	ret._v = w1 * v2 + w2 * v1 + Vec3::cross(v1, v2);
	return ret;
}
Quat Quat::operator*(const Vec3& vec) const
{
	Quat temp;
	temp._s = 0.0f;
	temp._v = vec;
	return (*this) * temp;
}
void Quat::conjugate()
{
	_v *= -1.0f;
}
Quat Quat::conjugated() const
{
	Quat ret = *this;
	ret.conjugate();
	return ret;
}
Mat4 Quat::toMat4() const
{
	float w = _s;
	float x = _v.x;
	float y = _v.y;
	float z = _v.z;
	float m[4][4] = {};

	m[0][0] = 1.0f - 2 * (y * y + z * z);
	m[0][1] = 2 * (x * y - w * z);
	m[0][2] = 2 * (x * z + w * y);
	m[1][0] = 2 * (x * y + w * z);
	m[1][1] = 1.0f - 2 * (x * x + z * z);
	m[1][2] = 2 * (y * z - w * x);
	m[2][0] = 2 * (x * z - w * y);
	m[2][1] = 2 * (y * z + w * x);
	m[2][2] = 1.0f - 2 * (x * x + y * y);
	m[3][3] = 1.0f;
	return Mat4(m);
}
#pragma endregion