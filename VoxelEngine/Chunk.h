#pragma once
#include <vector>
#include "Enums.h"
#include "Vec3.h"

class VertexArray;
class Chunk
{
public:
	static void update();

public:
	Chunk(const Vec3& origin);
	void draw();
	void remove(int x, int y, int z);
	void insert(int x, int y, int z);
private:
	void generateMesh();
	void calcHeight(uint64_t mask, int& start, int& height);
	int calcWidth(uint64_t mask, int i, uint64_t copy[CHUNK_SIZE]);

	Vec3 _origin;
	VertexArray* _va = nullptr;
	uint64_t _vx[CHUNK_SIZE][CHUNK_SIZE] = {};
	uint64_t _vy[CHUNK_SIZE][CHUNK_SIZE] = {};
};