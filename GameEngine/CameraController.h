#pragma once
#include "Component.h"
class CameraController : public Component
{
public:
	CameraController(Object* obj) : Component(obj) {}
protected:
	virtual void update(float dt) override;
private:
};