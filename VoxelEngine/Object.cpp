#include "Object.h"
#include "Component.h"
#include "MeshRenderer.h"


#pragma region public
Object::Object()
{
	_renderer = new MeshRenderer(this);
}
Object::~Object()
{
	for (Component* comp : _components)
		delete comp;
	delete _renderer;
}
void Object::removeComponent(Component* component)
{
	auto it = std::find(_components.begin(), _components.end(), component);
	if (it == _components.end())
		return;
	delete (*it);
	_components.erase(it);
}
void Object::update(float dt)
{
	transform.update();
	for (Component* comp : _components)
		if (comp->enabled())
			comp->update(dt);
	_renderer->draw();
}
void Object::active(bool flag)
{
	if (_active == flag)
		return;
	_active = flag;
	for (Component* comp : _components)
	{
		if (!comp->enabled())
			continue;
		if (_active)
			comp->onEnable();
		else
			comp->onDisable();
	}
}
void Object::mesh(Mesh* value)
{
	_renderer->mesh(value);
}
#pragma endregion



#pragma region private
#pragma endregion