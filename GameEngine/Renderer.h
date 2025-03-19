#pragma once
#include <SDL2/SDL.h>
#include "Component.h"

class Object;
class Renderer : public Component
{
public:
protected:
	Renderer(Object* gameObject);
	virtual ~Renderer() = default;
	Renderer(const Renderer& other) = delete;

	SDL_Renderer* const _renderer = nullptr;
};