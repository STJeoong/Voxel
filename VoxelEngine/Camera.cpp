#include "Camera.h"
#include "Vec3.h"
#include "Object.h"
#include "Structs.h"
#include "GameEngine.h"
#include "Shader.h"
#include <cmath>

#pragma region public
void Camera::fov(float value)
{
	if (value <= 0.0f || value >= PI)
		return;
	_fov = value;
}
void Camera::near(float value)
{
	if (value <= 0.0f)
		return;
	_near = value;
	if (_near > _far)
		_far = _near + 0.01f;
}
void Camera::far(float value)
{
	if (value <= 0.0f || value < _near + 0.01f)
		return;
	_far = value;
}
#pragma endregion



#pragma region protected
void Camera::update(float dt)
{
	const EngineConfig& config = GameEngine::config();

	// view matrix (look at)
	const Vec3& k = _obj.transform.forward();
	const Vec3& i = _obj.transform.right();
	const Vec3& j = _obj.transform.up();
	Vec3 t = {};
	t.x = -Vec3::dot(i, _obj.transform.pos);
	t.y = -Vec3::dot(j, _obj.transform.pos);
	t.z = -Vec3::dot(k, _obj.transform.pos);

	Mat4 view;
	Mat4 proj;
	{
		float m[4][4] =
		{
			i.x, i.y, i.z, t.x,
			j.x, j.y, j.z, t.y,
			k.x, k.y, k.z, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		view = Mat4(m);
	}

	// projection matrix
	float yScale = 1.0f / tan(_fov / 2.0f);
	float xScale = yScale * (config.height / (float)config.width);
	{
		float m[4][4] =
		{
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, _far / (_far - _near), (_far * _near) / (_near - _far),
			0.0f, 0.0f, 1.0f, 0.0f
		};
		proj = Mat4(m);
	}
	_viewProj = proj * view;
	GameEngine::shader()->setMat4f("viewProj", _viewProj.raw());
}
#pragma endregion