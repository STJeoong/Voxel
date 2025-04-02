#include "MeshRenderer.h"
#include <glad/glad.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "Object.h"
#include "GameEngine.h"
#include "Shader.h"
#include "Texture.h"

#pragma region public
void MeshRenderer::draw()
{
	if (_mesh == nullptr)
		return;

	Shader* shader = GameEngine::shader();
	shader->setMat4f("transform", _obj.transform.raw());
	_mesh->va()->setActive();
	const std::vector<Texture*> textures = _mesh->textures();
	for (int i = 0; i < textures.size(); ++i)
		textures[i]->setTexUnit(i);
	glDrawElements(GL_TRIANGLES, _mesh->va()->iSize(), GL_UNSIGNED_INT, nullptr);
}
#pragma endregion