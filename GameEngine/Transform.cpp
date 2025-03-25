#include "Transform.h"

void Transform::update()
{
	_mat = pos.asTranslation() * (rot.toMat4() * scale.asScale());
	_forward = Vec3::X + 2.0f * Vec3::cross(rot.v, Vec3::cross(rot.v, Vec3::X) + rot.s * Vec3::X);
}