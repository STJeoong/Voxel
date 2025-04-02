#pragma once
class Vec3;
class Vec4;
class Quat;
class Mat4
{
public:
	Mat4();
	Mat4(const Mat4& other);
	Mat4(float m[4][4]);
	Mat4& operator=(const Mat4& other);
	Vec4 operator*(const Vec4& vec4) const;
	Mat4 operator*(const Mat4& other) const;
	const float* raw() const { return _m[0]; }
private:
	float _m[4][4] = {};
};