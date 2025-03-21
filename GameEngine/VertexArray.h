#pragma once
class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int vSize, const unsigned int* idxs, unsigned int iSize);
	~VertexArray();
	void setActive();
private:
	unsigned int _vSize = 0;
	unsigned int _iSize = 0;
	unsigned int _vBuf = 0;
	unsigned int _iBuf = 0;
	unsigned int _vArr = 0;
};