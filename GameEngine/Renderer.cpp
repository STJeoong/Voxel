#include "Renderer.h"
#include "GameEngine.h"
Renderer::Renderer(Object* obj) : Component(obj) , _renderer(GameEngine::renderer()) {}