#pragma once
#include "Quat.h"
#include "Vec3.h"
#include "Mat4.h"
class Transform
{
public:
	void update();
	const float* raw() const { return _mat.raw(); }
	const Vec3& forward() const { return _forward; }
	const Vec3& right() const { return _right; }
	const Vec3& up() const { return _up; }

	Vec3 pos;
	Quat rot;
	Vec3 scale = { 1.0f, 1.0f, 1.0f };
private:

	Vec3 _forward = Vec3::Z * -1.0f;
	Vec3 _right = Vec3::X;
	Vec3 _up = Vec3::Y;
	Mat4 _mat;
};