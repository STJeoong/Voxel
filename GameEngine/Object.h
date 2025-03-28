#pragma once
#include <vector>
#include "Transform.h"

class Mesh;
class MeshRenderer;
class Component;
class Object
{
public:
	Object();
	~Object();
	template<typename T, typename... Args>
	T* addComponent(Args&&... args);
	template<typename T>
	T* getComponent();
	void removeComponent(Component* component);

	void update(float dt);
	bool active() const { return _active; }
	void active(bool flag);
	void mesh(Mesh* value);

	Transform transform;
private:

	std::vector<Component*> _components;
	bool _active = true;
	MeshRenderer* _renderer = nullptr;
};



template<typename T, typename ...Args>
inline T* Object::addComponent(Args && ...args)
{
	T* ret = new T(this, std::forward<Args>(args)...);
	_components.push_back(ret);
	return ret;
}
template<typename T>
inline T* Object::getComponent()
{
	T* ret = nullptr;
	for (Component* comp : _components)
		if ((ret = dynamic_cast<T*>(comp)) != nullptr)
			return ret;
	return nullptr;
}