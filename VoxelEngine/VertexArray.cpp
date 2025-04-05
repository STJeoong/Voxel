#include "VertexArray.h"
#include <glad/glad.h>

#pragma region public
VertexArray::VertexArray(const float* verts, unsigned int vSize, const unsigned int* idxs, unsigned int iSize) : _vSize(vSize), _iSize(iSize)
{
	glGenVertexArrays(1, &_vArr);
	glBindVertexArray(_vArr);

	glGenBuffers(1, &_vBuf);
	glBindBuffer(GL_ARRAY_BUFFER, _vBuf);
	glBufferData(GL_ARRAY_BUFFER, vSize * 6 * sizeof(float), verts, GL_STATIC_DRAW); // 버퍼 복사

	glGenBuffers(1, &_iBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize * sizeof(unsigned int), idxs, GL_STATIC_DRAW); // 버퍼 복사

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); // 만약 GL_TRUE로 하면 값을 정규화함. int, byte같은 것은 GL_TRUE로 해줘야됨.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
}
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &_vBuf);
	glDeleteBuffers(1, &_iBuf);
	glDeleteVertexArrays(1, &_vArr);
}
void VertexArray::setActive()
{
	glBindVertexArray(_vArr);
}
#pragma endregion