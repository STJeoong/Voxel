#pragma once

class Vec3;
class Vec4
{
	friend class Mat4;
public:
	Vec4() = default;
	Vec4(const Vec4& other);
	Vec4& operator=(const Vec4& other);
	Vec4(float x, float y, float z, float w);
	Vec4(float v[4]);
	Vec4(const Vec3& vec3, float w = 1.0f);

	float len() const;
	float squaredLen() const;
	void normalize();
	Vec4 normalized() const;
	Vec4 operator*(float val) const;
	Vec4 operator+(const Vec4& other) const;
	Vec4 operator-(const Vec4& other) const;
	Vec4& operator+=(const Vec4& other);
	Vec4& operator-=(const Vec4& other);
	bool operator==(const Vec4& other) const;

	float& x;
	float& y;
	float& z;
	float& w;
private:
	float _v[4] = {};
};