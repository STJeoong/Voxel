#pragma once
#include <unordered_map>


class Mat4;
class Vec3
{
	friend class Vec4;

	friend Vec3 operator*(float val, const Vec3& vec);
public:
	static float dot(const Vec3& v1, const Vec3& v2);
	static Vec3 cross(const Vec3& v1, const Vec3& v2);

	static const Vec3 X;
	static const Vec3 Y;
	static const Vec3 Z;
public:
	Vec3() : x(_v[0]), y(_v[1]), z(_v[2]) {}
	Vec3(const Vec3& other);
	Vec3& operator=(const Vec3& other);
	Vec3(float x, float y, float z);
	Vec3(float v[3]);
	float len() const;
	float squaredLen() const;
	void normalize();
	Vec3 normalized() const;
	Mat4 asScale() const;
	Mat4 asTranslation() const;
	Vec3 operator*(float val) const;
	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3& operator*=(float val);
	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	bool operator==(const Vec3& other) const;
	size_t operator()() const;
	

	float& x;
	float& y;
	float& z;
private:
	float _v[3] = {};
};

namespace std
{
	template <>
	struct hash<Vec3>
	{
		size_t operator()(const Vec3& v) const
		{
			hash<float> func;
			return func(v.x) ^ (func(v.y) << 1) ^ (func(v.z) << 2);
		}
	};
}

using Point3 = Vec3;