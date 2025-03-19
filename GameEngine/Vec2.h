#pragma once
class Vec2
{
public:
	static float dot(const Vec2& v1, const Vec2& v2);

public:
	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {}
	float len() const;
	void normalize();
	Vec2 normalized() const;
	Vec2 operator*(float val) const;
	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	bool operator==(const Vec2& other) const;

	float x = 0.0f;
	float y = 0.0f;
};

using Point2 = Vec2;