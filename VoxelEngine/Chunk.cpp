#include "Chunk.h"
#include "GameEngine.h"
#include "Structs.h"
#include "Camera.h"
#include "PerlinNoise.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>
#include <boost/asio.hpp>

static std::unordered_map<Vec3, Chunk*> s_mp;
static Chunk* s_chunks[101][101] = {};
static boost::asio::thread_pool s_pool(4);

#pragma region public static
void Chunk::update()
{
	/*for (int i = -50; i <= 50; ++i)
		for (int j = -50; j <= 50; ++j)
		{
			if (s_chunks[i + 50][j + 50] == nullptr)
				s_chunks[i + 50][j + 50] = new Chunk({ (float)i * CHUNK_SIZE, 0, (float)j * CHUNK_SIZE });
			s_chunks[i + 50][j + 50]->draw();
		}*/

	int load = GameEngine::config().chunkLoad;
	int vs = GameEngine::config().voxelSize;
	int gridX = static_cast<int>(Camera::transform.pos.x / (CHUNK_SIZE * vs));
	int gridZ = static_cast<int>(Camera::transform.pos.z / (CHUNK_SIZE * vs));
	for (int i = gridX - load / 2; i <= gridX + load / 2; ++i)
		for (int j = gridZ - load / 2; j <= gridZ + load / 2; ++j)
		{
			Vec3 key = { static_cast<float>(i * CHUNK_SIZE * vs), 0, static_cast<float>(j * CHUNK_SIZE * vs) };
			Chunk* chunk = nullptr;
			if (s_mp.find(key) == s_mp.end())
			{
				chunk = new Chunk(key);
				s_mp[key] = chunk;
			}
			else
				chunk = s_mp[key];

			if (!chunk->_isLoaded && !chunk->_isLoading)
			{
				chunk->_isLoading = true;
				boost::asio::post(s_pool, [chunk]() { chunk->load(); chunk->generateMesh(); chunk->_isLoaded = true; });
			}
			else if (chunk->_isLoaded && !chunk->_drawable)
			{
				chunk->_va = new VertexArray(chunk->_vertices.data(), chunk->_vertices.size() / 6, chunk->_indices.data(), chunk->_indices.size());
				chunk->_drawable = true;
			}
			else if (chunk->_drawable)
				chunk->draw();
		}
}
#pragma endregion




#pragma region public
Chunk::Chunk(const Vec3& origin) : _origin(origin) { }
Chunk::~Chunk() { delete _va; }
#pragma endregion




#pragma region private
void Chunk::load()
{
	static siv::PerlinNoise perlin(1234);
	int vs = GameEngine::config().voxelSize;
	for (int x = 0; x < CHUNK_SIZE; ++x)
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			double noise = perlin.octave2D_01((_origin.x + x * vs) * 0.001, (_origin.z + z * vs) * 0.001, 4);
			int height = static_cast<int>(noise * CHUNK_SIZE);
			for (int y = 0; y < height; ++y)
				_vx[y][z] |= (1ull << x);
			_vy[x][z] = (1ull << height) - 1;
		}
}
void Chunk::draw()
{
	_va->setActive();
	Shader* shader = GameEngine::shader();
	shader->setMat4f("transform", _origin.asTranslation().raw());
	glDrawElements(GL_TRIANGLES, _va->iSize(), GL_UNSIGNED_INT, nullptr);
}
void Chunk::generateMesh()
{
	int vs = GameEngine::config().voxelSize;
	_vertices.clear();
	_indices.clear();

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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), {vx * vs, vy * vs, vz * vs, -1, 0, 0});
			_vertices.insert(_vertices.end(), {vx * vs, vy * vs, (vz + width) * vs, -1, 0, 0});
			_vertices.insert(_vertices.end(), {vx * vs, (vy + height) * vs, (vz + width) * vs, -1, 0, 0});
			_vertices.insert(_vertices.end(), {vx * vs, (vy + height) * vs, vz * vs, -1, 0, 0});
			_indices.insert(_indices.end(), {idx, idx + 1, idx + 2});
			_indices.insert(_indices.end(), {idx, idx + 2, idx + 3});
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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, vz * vs, 1, 0, 0 });
			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, (vz - width) * vs, 1, 0, 0 });
			_vertices.insert(_vertices.end(), { vx * vs, (vy + height) * vs, (vz - width) * vs, 1, 0, 0 });
			_vertices.insert(_vertices.end(), { vx * vs, (vy + height) * vs, vz * vs, 1, 0, 0 });
			_indices.insert(_indices.end(), { idx, idx + 1, idx + 2 });
			_indices.insert(_indices.end(), { idx, idx + 2, idx + 3 });
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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, vz * vs, 0, 0, -1 });
			_vertices.insert(_vertices.end(), { (vx - width) * vs, vy * vs, vz * vs, 0, 0, -1 });
			_vertices.insert(_vertices.end(), { (vx - width) * vs, (vy + height) * vs, vz * vs, 0, 0, -1 });
			_vertices.insert(_vertices.end(), { vx * vs, (vy + height) * vs, vz * vs, 0, 0, -1 });
			_indices.insert(_indices.end(), { idx, idx + 1, idx + 2 });
			_indices.insert(_indices.end(), { idx, idx + 2, idx + 3 });
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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, vz * vs, 0, 0, 1 });
			_vertices.insert(_vertices.end(), { (vx + width) * vs, vy * vs, vz * vs, 0, 0, 1 });
			_vertices.insert(_vertices.end(), { (vx + width) * vs, (vy + height) * vs, vz * vs, 0, 0, 1 });
			_vertices.insert(_vertices.end(), { vx * vs, (vy + height) * vs, vz * vs, 0, 0, 1 });
			_indices.insert(_indices.end(), { idx, idx + 1, idx + 2 });
			_indices.insert(_indices.end(), { idx, idx + 2, idx + 3 });
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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, vz * vs, 0, -1, 0 });
			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, (vz - width) * vs, 0, -1, 0 });
			_vertices.insert(_vertices.end(), { (vx + height) * vs, vy * vs, (vz - width) * vs, 0, -1, 0 });
			_vertices.insert(_vertices.end(), { (vx + height) * vs, vy * vs, vz * vs, 0, -1, 0 });
			_indices.insert(_indices.end(), { idx, idx + 1, idx + 2 });
			_indices.insert(_indices.end(), { idx, idx + 2, idx + 3 });
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
			unsigned int idx = static_cast<unsigned int>(_vertices.size()) / 6;

			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, vz * vs, 0, 1, 0 });
			_vertices.insert(_vertices.end(), { vx * vs, vy * vs, (vz + width) * vs, 0, 1, 0 });
			_vertices.insert(_vertices.end(), { (vx + height) * vs, vy * vs, (vz + width) * vs, 0, 1, 0 });
			_vertices.insert(_vertices.end(), { (vx + height) * vs, vy * vs, vz * vs, 0, 1, 0 });
			_indices.insert(_indices.end(), { idx, idx + 1, idx + 2 });
			_indices.insert(_indices.end(), { idx, idx + 2, idx + 3 });
		}
	}
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