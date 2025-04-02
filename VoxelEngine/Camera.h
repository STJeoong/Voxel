#pragma once
#include "Enums.h"
#include "Mat4.h"
#include "Component.h"
class Camera : public Component
{
public:
	Camera(Object* obj) : Component(obj) {}
	const Mat4& viewProj() const { return _viewProj; }
	void fov(float value);
	void near(float value);
	void far(float value);
protected:
	virtual void update(float dt) override;
private:
	float _fov = PI / 3.0f; // field of view
	float _near = 0.3f;
	float _far = 1000.0f;
	Mat4 _viewProj = {};
};