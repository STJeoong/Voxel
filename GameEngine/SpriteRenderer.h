#pragma once
#include "Renderer.h"

class Object;
class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer(Object* obj) : Renderer(obj) {}
protected:
	virtual void update(float dt) override;
};