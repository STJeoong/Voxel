#include "Object.h"
#include "Component.h"


#pragma region public
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
	for (Component* comp : _components)
		if (comp->enabled())
			comp->update(dt);
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
#pragma endregion

#pragma region private
#pragma endregion