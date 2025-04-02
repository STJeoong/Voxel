#include "CameraController.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"
#include <algorithm>
#include "Enums.h"

#include <iostream>
#include <format>
void CameraController::update(float dt)
{
	if (Input::getKey(GLFW_KEY_W))
		_obj.transform.pos += _obj.transform.forward() * 3.0f;
	else if (Input::getKey(GLFW_KEY_S))
		_obj.transform.pos -= _obj.transform.forward() * 3.0f;
	else if (Input::getKey(GLFW_KEY_A))
		_obj.transform.pos -= _obj.transform.right() * 3.0f;
	else if (Input::getKey(GLFW_KEY_D))
		_obj.transform.pos += _obj.transform.right() * 3.0f;
	else if (Input::getKey(GLFW_KEY_Q))
		_obj.transform.pos -= _obj.transform.up() * 3.0f;
	else if (Input::getKey(GLFW_KEY_E))
		_obj.transform.pos += _obj.transform.up() * 3.0f;
	
	_hRot -= Input::getMouseAxisX() * 0.001f;
	_vRot += Input::getMouseAxisY() * 0.001f;
	_vRot = std::clamp(_vRot, -PI / 2, PI / 2);
	_obj.transform.rot = { _vRot, _hRot, 0.0f };
}