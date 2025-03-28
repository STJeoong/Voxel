#include "Transform.h"

void Transform::update()
{
	_mat = pos.asTranslation() * (rot.toMat4() * scale.asScale());
	/*_right = Vec3::X * -1.0f + 2.0f * Vec3::cross(rot.v(), Vec3::cross(rot.v(), Vec3::X * -1.0f) + rot.s() * Vec3::X * -1.0f);
	_up = Vec3::Y + 2.0f * Vec3::cross(rot.v(), Vec3::cross(rot.v(), Vec3::Y) + rot.s() * Vec3::Y);
	_forward = Vec3::Z + 2.0f * Vec3::cross(rot.v(), Vec3::cross(rot.v(), Vec3::Z) + rot.s() * Vec3::Z);*/
	_right = ((rot * (Vec3::X)) * rot.conjugated()).v(); // 왜 X축 방향이 오른쪽이지? 이러면 왼손좌표계잖아.
	_up = ((rot * Vec3::Y)* rot.conjugated()).v();
	_forward = ((rot * (Vec3::Z * -1.0f)) * rot.conjugated()).v();
}