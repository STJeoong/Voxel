#include "Component.h"
#include "Object.h"

void Component::enabled(bool flag)
{
	if (_enabled == flag)
		return;
	_enabled = flag;
	if (!_obj->active())
		return;
	if (_enabled)
		this->onEnable();
	else
		this->onDisable();
}