#pragma once
#include "Vec3.h"
class Mat4;
class Quat
{
public:
	static Quat slerp(const Quat& q1, const Quat& q2, float f);

public:
	Quat() : s(_s), v(_v) {}
	Quat(const Quat& other) : s(_s), v(_v), _s(other.s), _v(other.v) {}
	Quat(const Vec3& axis, float radian);
	Quat& operator=(const Quat& other);
	Quat& operator*=(const Quat& other);
	Quat operator*(const Quat& other) const;
	Quat operator*(const Vec3& vec) const;
	void conjugate();
	Quat conjugated() const;
	Mat4 toMat4() const;

	const float& s;
	const Vec3& v;
private:

	float _s = 1.0f;
	Vec3 _v = {};
};