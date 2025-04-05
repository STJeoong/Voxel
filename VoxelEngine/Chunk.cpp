#include "Chunk.h"
#include "GameEngine.h"
#include "Structs.h"
#include "Camera.h"
#include "PerlinNoise.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma region public static
void Chunk::update()
{
}
#pragma endregion




#pragma region public
Chunk::Chunk(const Vec3& origin) : _origin(origin)
{
	static siv::PerlinNoise perlin(1234);
	for (int x = 0; x < CHUNK_SIZE; ++x)
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			double noise = perlin.octave2D_01((origin.x + x) * 0.001, (origin.z + z) * 0.001, 4);
			int height = static_cast<int>(noise * CHUNK_SIZE);
			for (int y = 0; y < height; ++y)
				_vx[y][z] |= (1ull << x);
			_vy[x][z] = (1ull << height) - 1;
		}

	this->generateMesh();
}
void Chunk::remove(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
		throw std::runtime_error("remove voxel error (out of range)");

	if (!(_vx[y][z] & (1ull << x)))
		return;

	_vx[y][z] |= (1ull << x);
	_vy[x][z] |= (1ull << y);
	this->generateMesh();
}
void Chunk::insert(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
		throw std::runtime_error("insert voxel error (out of range)");

	if (_vx[y][z] & (1ull << x))
		return;

	_vx[y][z] &= ~(1ull << x);
	_vy[x][z] &= ~(1ull << y);
	this->generateMesh();
}
#pragma endregion




#pragma region private
void Chunk::generateMesh()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;


	// -x축
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (x != 0)
				copy[z] = _vy[x][z] & ~_vy[x - 1][z];
			else
				copy[z] = _vy[x][z];
		}

		for (int z = 0; z < CHUNK_SIZE;)
		{
			if (copy[z] == 0)
			{
				++z;
				continue;
			}

			uint64_t mask = copy[z] & ~(copy[z] + (copy[z] & (~copy[z] + 1)));
			int width = this->calcWidth(mask, z, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>(x - CHUNK_SIZE / 2);
			float vy = static_cast<float>(start - CHUNK_SIZE / 2);
			float vz = static_cast<float>(z - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), {vx, vy, vz, -1, 0, 0});
			vertices.insert(vertices.end(), {vx, vy, vz + width, -1, 0, 0});
			vertices.insert(vertices.end(), {vx, vy + height, vz + width, -1, 0, 0});
			vertices.insert(vertices.end(), {vx, vy + height, vz, -1, 0, 0});
			indices.insert(indices.end(), {idx, idx + 1, idx + 2});
			indices.insert(indices.end(), {idx, idx + 2, idx + 3});
		}
	}

	// +x축
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (x != CHUNK_SIZE - 1)
				copy[z] = _vy[x][CHUNK_SIZE - 1 - z] & ~_vy[x + 1][CHUNK_SIZE - 1 - z];
			else
				copy[z] = _vy[x][CHUNK_SIZE - 1 - z];
		}

		for (int z = 0; z < CHUNK_SIZE;)
		{
			if (copy[z] == 0)
			{
				++z;
				continue;
			}

			uint64_t mask = copy[z] & ~(copy[z] + (copy[z] & (~copy[z] + 1)));
			int width = this->calcWidth(mask, z, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>(x + 1 - CHUNK_SIZE / 2);
			float vy = static_cast<float>(start - CHUNK_SIZE / 2);
			float vz = static_cast<float>((CHUNK_SIZE - z) - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), { vx, vy, vz, 1, 0, 0 });
			vertices.insert(vertices.end(), { vx, vy, vz - width, 1, 0, 0 });
			vertices.insert(vertices.end(), { vx, vy + height, vz - width, 1, 0, 0 });
			vertices.insert(vertices.end(), { vx, vy + height, vz, 1, 0, 0 });
			indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
			indices.insert(indices.end(), { idx, idx + 2, idx + 3 });
		}
	}

	// -z축 방향
	for (int z = 0; z < CHUNK_SIZE; ++z)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			if (z != 0)
				copy[x] = _vy[CHUNK_SIZE - 1 - x][z] & ~_vy[CHUNK_SIZE - 1 - x][z - 1];
			else
				copy[x] = _vy[CHUNK_SIZE - 1 - x][z];
		}

		for (int x = 0; x < CHUNK_SIZE;)
		{
			if (copy[x] == 0)
			{
				++x;
				continue;
			}

			uint64_t mask = copy[x] & ~(copy[x] + (copy[x] & (~copy[x] + 1)));
			int width = this->calcWidth(mask, x, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>((CHUNK_SIZE - x) - CHUNK_SIZE / 2);
			float vy = static_cast<float>(start - CHUNK_SIZE / 2);
			float vz = static_cast<float>(z - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), { vx, vy, vz, 0, 0, -1 });
			vertices.insert(vertices.end(), { vx - width, vy, vz, 0, 0, -1 });
			vertices.insert(vertices.end(), { vx - width, vy + height, vz, 0, 0, -1 });
			vertices.insert(vertices.end(), { vx, vy + height, vz, 0, 0, -1 });
			indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
			indices.insert(indices.end(), { idx, idx + 2, idx + 3 });
		}
	}

	// +z축 방향
	for (int z = CHUNK_SIZE - 1; z >= 0; --z)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			if (z != CHUNK_SIZE - 1)
				copy[x] = _vy[x][z] & ~_vy[x][z + 1];
			else
				copy[x] = _vy[x][z];
		}

		for (int x = 0; x < CHUNK_SIZE;)
		{
			if (copy[x] == 0)
			{
				++x;
				continue;
			}

			uint64_t mask = copy[x] & ~(copy[x] + (copy[x] & (~copy[x] + 1)));
			int width = this->calcWidth(mask, x, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>(x - CHUNK_SIZE / 2);
			float vy = static_cast<float>(start - CHUNK_SIZE / 2);
			float vz = static_cast<float>(z + 1 - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), { vx, vy, vz, 0, 0, 1 });
			vertices.insert(vertices.end(), { vx + width, vy, vz, 0, 0, 1 });
			vertices.insert(vertices.end(), { vx + width, vy + height, vz, 0, 0, 1 });
			vertices.insert(vertices.end(), { vx, vy + height, vz, 0, 0, 1 });
			indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
			indices.insert(indices.end(), { idx, idx + 2, idx + 3 });
		}
	}

	// -y축
	for (int y = 0; y < CHUNK_SIZE; ++y)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (y != 0)
				copy[z] = _vx[y][CHUNK_SIZE - 1 - z] & ~_vx[y - 1][CHUNK_SIZE - 1 - z];
			else
				copy[z] = _vx[y][CHUNK_SIZE - 1 - z];
		}

		for (int z = 0; z < CHUNK_SIZE;)
		{
			if (copy[z] == 0)
			{
				++z;
				continue;
			}

			uint64_t mask = copy[z] & ~(copy[z] + (copy[z] & (~copy[z] + 1)));
			int width = this->calcWidth(mask, z, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>(start - CHUNK_SIZE / 2);
			float vy = static_cast<float>(y - CHUNK_SIZE / 2);
			float vz = static_cast<float>((CHUNK_SIZE - z) - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), { vx, vy, vz, 0, -1, 0 });
			vertices.insert(vertices.end(), { vx, vy, vz - width, 0, -1, 0 });
			vertices.insert(vertices.end(), { vx + height, vy, vz - width, 0, -1, 0 });
			vertices.insert(vertices.end(), { vx + height, vy, vz, 0, -1, 0 });
			indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
			indices.insert(indices.end(), { idx, idx + 2, idx + 3 });
		}
	}

	// +y축
	for (int y = 0; y < CHUNK_SIZE; ++y)
	{
		uint64_t copy[CHUNK_SIZE] = {};
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (y != 0)
				copy[z] = _vx[CHUNK_SIZE - 1 - y][z] & ~_vx[CHUNK_SIZE - y][z];
			else
				copy[z] = _vx[CHUNK_SIZE - 1 - y][z];
		}

		for (int z = 0; z < CHUNK_SIZE;)
		{
			if (copy[z] == 0)
			{
				++z;
				continue;
			}

			uint64_t mask = copy[z] & ~(copy[z] + (copy[z] & (~copy[z] + 1)));
			int width = this->calcWidth(mask, z, copy);
			int start, height;
			this->calcHeight(mask, start, height);

			// vertex position
			float vx = static_cast<float>(start - CHUNK_SIZE / 2);
			float vy = static_cast<float>((CHUNK_SIZE - y) - CHUNK_SIZE / 2);
			float vz = static_cast<float>(z - CHUNK_SIZE / 2);
			unsigned int idx = static_cast<unsigned int>(vertices.size()) / 6;

			vertices.insert(vertices.end(), { vx, vy, vz, 0, 1, 0 });
			vertices.insert(vertices.end(), { vx, vy, vz + width, 0, 1, 0 });
			vertices.insert(vertices.end(), { vx + height, vy, vz + width, 0, 1, 0 });
			vertices.insert(vertices.end(), { vx + height, vy, vz, 0, 1, 0 });
			indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
			indices.insert(indices.end(), { idx, idx + 2, idx + 3 });
		}
	}

	delete _va;
	_va = new VertexArray(vertices.data(), vertices.size() / 6, indices.data(), indices.size());
}
void Chunk::draw()
{
	_va->setActive();
	Shader* shader = GameEngine::shader();
	shader->setMat4f("transform", _origin.asTranslation().raw());
	glDrawElements(GL_TRIANGLES, _va->iSize(), GL_UNSIGNED_INT, nullptr);
}
void Chunk::calcHeight(uint64_t mask, int& start, int& height)
{
	start = height = -1;
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		if (start == -1 && (mask & 1))
			start = i;
		else if (start != -1 && !(mask & 1))
		{
			height = i - start;
			return;
		}
		mask >>= 1;
	}
	height = CHUNK_SIZE - start;
	return;
}
int Chunk::calcWidth(uint64_t mask, int i, uint64_t copy[CHUNK_SIZE])
{
	int width = 0;
	for (; i < CHUNK_SIZE; ++i)
	{
		if (mask != (mask & copy[i]))
			break;
		copy[i] &= ~mask;
		++width;
	}
	return width;
}
#pragma endregion