#pragma once
#include "Vec3.h"
class Mat4;
class Quat
{
public:
	static Quat slerp(const Quat& q1, const Quat& q2, float f);

public:
	Quat() = default;
	Quat(const Vec3& axis, float radian);
	Quat(const Vec3& euler);
	Quat& operator=(const Quat& other);
	Quat& operator*=(const Quat& other);
	Quat operator*(const Quat& other) const;
	Quat operator*(const Vec3& vec) const;
	void conjugate();
	Quat conjugated() const;
	Mat4 toMat4() const;
	float s() const { return _s; }
	const Vec3& v() const { return _v; }
private:

	float _s = 1.0f;
	Vec3 _v = {};
};