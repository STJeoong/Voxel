#include "CameraController.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"

void CameraController::update(float dt)
{
	if (Input::getKey(GLFW_KEY_W))
		_obj.transform.pos += _obj.transform.forward();
	else if (Input::getKey(GLFW_KEY_S))
		_obj.transform.pos -= _obj.transform.forward();
	else if (Input::getKey(GLFW_KEY_A))
		_obj.transform.pos -= _obj.transform.right();
	else if (Input::getKey(GLFW_KEY_D))
		_obj.transform.pos += _obj.transform.right();
	else if (Input::getKey(GLFW_KEY_Q))
		_obj.transform.pos += _obj.transform.up();
	else if (Input::getKey(GLFW_KEY_E))
		_obj.transform.pos -= _obj.transform.up();


	//if (Input::getKey(GLFW_KEY_W))
	//	_obj.transform.pos += Vec3::Z * 0.1f;
	//else if (Input::getKey(GLFW_KEY_S))
	//	_obj.transform.pos -= Vec3::Z * 0.1f;
	//else if (Input::getKey(GLFW_KEY_A))
	//	_obj.transform.pos += Vec3::X * 0.1f;
	//else if (Input::getKey(GLFW_KEY_D))
	//	_obj.transform.pos -= Vec3::X * 0.1f;
	//else if (Input::getKey(GLFW_KEY_Q))
	//	_obj.transform.pos += Vec3::Y * 0.1f;
	//else if (Input::getKey(GLFW_KEY_E))
	//	_obj.transform.pos -= Vec3::Y * 0.1f;
}