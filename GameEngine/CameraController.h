#pragma once
#include "Component.h"
class CameraController : public Component
{
public:
	CameraController(Object* obj) : Component(obj) {}
protected:
	virtual void update(float dt) override;
private:

	float _hRot = 0.0f;
	float _vRot = 0.0f;
};