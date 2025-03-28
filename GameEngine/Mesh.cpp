#include "Mesh.h"
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include "VertexArray.h"
#include "Texture.h"

using json = nlohmann::json;

static std::map<std::string, Mesh*> s_mesh;

#pragma region public static
Mesh* Mesh::get(const std::string& fileName)
{
	if (s_mesh.find(fileName) == s_mesh.end())
		s_mesh[fileName] = new Mesh(fileName);
	return s_mesh[fileName];
}
#pragma endregion




#pragma region private
Mesh::Mesh(const std::string& fileName)
{
	std::ifstream fstream(fileName);
	json j = json::parse(fstream);

	std::vector<std::string> textures;
	j["textures"].get_to(textures);
	for (const std::string& fileName : textures)
		_textures.push_back(Texture::get(fileName));

	std::vector<std::vector<float>> vertices;
	j["vertices"].get_to(vertices);
	float* vert = new float[vertices.size() * 8];
	for (int i = 0; i < vertices.size(); ++i)
		for (int j = 0; j < 8; ++j)
			vert[i * 8 + j] = vertices[i][j];

	std::vector<std::vector<int>> indices;
	j["indices"].get_to(indices);
	unsigned int* idxs = new unsigned int[indices.size() * 3];
	for (int i = 0; i < indices.size(); ++i)
		for (int j = 0; j < 3; ++j)
			idxs[i * 3 + j] = indices[i][j];

	_va = new VertexArray(vert, vertices.size(), idxs, indices.size() * 3);
	delete[] vert;
	delete[] idxs;
}
#pragma endregion