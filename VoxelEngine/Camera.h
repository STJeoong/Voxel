#pragma once
#include "Enums.h"
#include "Mat4.h"
#include "Transform.h"
class Camera
{
	friend class GameEngine;
public:
	static void init();
	static const Mat4& viewProj();
	static void fov(float value);
	static void near(float value);
	static void far(float value);

	static Transform transform;
private:
	static void update(float dt);
	static void move();
};