#pragma once
#include <string>
#include <vector>
class VertexArray;
class Texture;
class Mesh
{
public:
	static Mesh* get(const std::string& fileName);

public:
	VertexArray* va() const { return _va; }
	const std::vector<Texture*> textures() const { return _textures; }
private:
	Mesh(const std::string& fileName);

	VertexArray* _va = nullptr;
	std::vector<Texture*> _textures;
};