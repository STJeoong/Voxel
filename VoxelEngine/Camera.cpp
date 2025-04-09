#include "Camera.h"
#include "Vec3.h"
#include "Structs.h"
#include "GameEngine.h"
#include "Shader.h"
#include "Input.h"
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static Mat4 s_viewProj = {};
static float s_fov = PI / 3.0f;
static float s_near = 0.3f;
static float s_far = 1000.0f;
static float s_camSpeed = 0.1f;
static float s_mouseSensitivity = 0.1f;
Transform Camera::transform = {};

#pragma region static public
void Camera::init()
{
	const EngineConfig& config = GameEngine::config();
	s_fov = config.fov;
	s_near = config.screenNear;
	s_far = config.screenFar;
	s_mouseSensitivity = config.mouseSensitivity;
	s_camSpeed = config.camSpeed;
}
const Mat4& Camera::viewProj() { return s_viewProj; }
void Camera::fov(float value)
{
	if (value <= 0.0f || value >= PI)
		return;
	s_fov = value;
}
void Camera::near(float value)
{
	if (value <= 0.0f)
		return;
	s_near = value;
	if (s_near > s_far)
		s_far = s_near + 0.01f;
}
void Camera::far(float value)
{
	if (value <= 0.0f || value < s_near + 0.01f)
		return;
	s_far = value;
}
#pragma endregion



#pragma region static private
void Camera::update(float dt)
{
	Camera::move();
	const EngineConfig& config = GameEngine::config();

	// view matrix (look at)
	const Vec3& k = transform.forward();
	const Vec3& i = transform.right();
	const Vec3& j = transform.up();
	Vec3 t = {};
	t.x = -Vec3::dot(i, transform.pos);
	t.y = -Vec3::dot(j, transform.pos);
	t.z = -Vec3::dot(k, transform.pos);

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
	float yScale = 1.0f / tan(s_fov / 2.0f);
	float xScale = yScale * (config.height / (float)config.width);
	{
		float m[4][4] =
		{
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, s_far / (s_far - s_near), (s_far * s_near) / (s_near - s_far),
			0.0f, 0.0f, 1.0f, 0.0f
		};
		proj = Mat4(m);
	}
	s_viewProj = proj * view;
	GameEngine::shader()->setMat4f("viewProj", s_viewProj.raw());
	GameEngine::shader()->setVec3f("camPos", transform.pos.x, transform.pos.y, transform.pos.z);
}
void Camera::move()
{
	static float s_hRot = 0.0f, s_vRot = 0.0f;

	if (Input::getKey(GLFW_KEY_W))
		transform.pos += transform.forward() * s_camSpeed;
	else if (Input::getKey(GLFW_KEY_S))
		transform.pos -= transform.forward() * s_camSpeed;
	else if (Input::getKey(GLFW_KEY_A))
		transform.pos -= transform.right() * s_camSpeed;
	else if (Input::getKey(GLFW_KEY_D))
		transform.pos += transform.right() * s_camSpeed;
	else if (Input::getKey(GLFW_KEY_Q))
		transform.pos -= transform.up() * s_camSpeed;
	else if (Input::getKey(GLFW_KEY_E))
		transform.pos += transform.up() * s_camSpeed;

	s_hRot -= Input::getMouseAxisX() * s_mouseSensitivity;
	s_vRot += Input::getMouseAxisY() * s_mouseSensitivity;
	s_vRot = std::clamp(s_vRot, -PI / 2, PI / 2);
	transform.rot = { s_vRot, s_hRot, 0.0f };
	transform.update();
}
#pragma endregion