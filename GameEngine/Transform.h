#pragma once
#include "Quat.h"
#include "Vec3.h"
#include "Mat4.h"
class Transform
{
public:
	Transform() : forward(_forward) {}
	void update();
	const float* raw() const { return _mat.raw(); }

	Vec3 pos;
	Quat rot;
	Vec3 scale = { 1.0f, 1.0f, 1.0f };
	const Vec3& forward;
private:

	Vec3 _forward;
	Mat4 _mat;
};