#pragma once
class VertexArray
{
public:
	// vSize : vertex attribute ������ 1���� count, iSize : index �ϳ��� 1���� count
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