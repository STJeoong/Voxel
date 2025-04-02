#pragma once

class Object;
class Mesh;
class MeshRenderer
{
public:
	MeshRenderer(Object* obj) : _obj(*obj) {}
	void draw();
	void mesh(Mesh* value) { _mesh = value; }
private:

	Object& _obj;
	Mesh* _mesh = nullptr;
};