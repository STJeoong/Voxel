#pragma once

class Object;
class Component
{
	friend class Object;
public:
	Component(Object* obj) : _obj(*obj) {}
	virtual ~Component() = default;
	bool enabled() const { return _enabled; }
	void enabled(bool flag);
protected:
	virtual void update(float dt) {}
	virtual void onEnable() {}
	virtual void onDisable() {}

	Object& _obj;
private:
	bool _enabled = true;
};