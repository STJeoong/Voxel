#pragma once
class Vec3
{
public:
	static float dot(const Vec3& v1, const Vec3& v2);
	static Vec3 cross(const Vec3& v1, const Vec3& v2);

public:
	Vec3() = default;
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	float len() const;
	void normalize();
	Vec3 normalized() const;
	Vec3 operator*(float val) const;
	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	bool operator==(const Vec3& other) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

using Point3 = Vec3;