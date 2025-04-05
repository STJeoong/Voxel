#pragma once
class VertexArray
{
public:
	// vSize : vertex attribute 묶음을 1개로 count, iSize : index 하나를 1개로 count
	VertexArray(const float* verts, unsigned int vSize, const unsigned int* idxs, unsigned int iSize);
	~VertexArray();
	void setActive();
	unsigned int vSize() const { return _vSize; }
	unsigned int iSize() const { return _iSize; }
private:
	unsigned int _vSize = 0;
	unsigned int _iSize = 0;
	unsigned int _vBuf = 0;
	unsigned int _iBuf = 0;
	unsigned int _vArr = 0;
};