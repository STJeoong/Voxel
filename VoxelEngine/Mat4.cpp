#include "Mat4.h"
#include "Vec4.h"
#include "Vec3.h"
#include "Quat.h"

#pragma region public
Mat4::Mat4()
{
	_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = 1.0f;
}
Mat4::Mat4(const Mat4& other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			_m[i][j] = other._m[i][j];
}
Mat4::Mat4(float m[4][4])
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			_m[i][j] = m[i][j];
}
Mat4& Mat4::operator=(const Mat4& other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			_m[i][j] = other._m[i][j];
	return *this;
}
Vec4 Mat4::operator*(const Vec4& vec4) const
{
	float v[4] = {};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			v[i] += _m[i][j] * vec4._v[j];
	return Vec4(v);
}
Mat4 Mat4::operator*(const Mat4& other) const
{
	float m[4][4] = {};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				m[i][j] += _m[i][k] * other._m[k][j];
	return Mat4(m);
}
#pragma endregion