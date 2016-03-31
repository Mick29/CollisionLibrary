#pragma once

class Vec2
{
public:
	Vec2(void);
	Vec2(float xp, float yp);
	Vec2(const Vec2 &rhs);
	~Vec2(void);

	bool operator==(const Vec2 &rhs) const;
	bool operator!=(const Vec2 &rhs) const;
	void operator+=(const Vec2 &rhs);
	void operator-=(const Vec2 &rhs);

	Vec2& operator=(const Vec2 &rhs);
	Vec2& operator*=(const Vec2 &rhs);
	Vec2& operator*=(const float scalar);

	Vec2 operator+(const Vec2 &rhs) const;
	Vec2 operator-(const Vec2 &rhs) const;
	Vec2 operator*(const Vec2 &rhs) const;
	Vec2 operator/(const Vec2 &rhs) const;

	Vec2 operator+(const float scalar) const;
	Vec2 operator-(const float scalar) const;
	Vec2 operator*(const float scalar) const;
	Vec2 operator/(const float scalar) const;

	float dotProduct(const Vec2 &rhs) const;
	float magnitude() const;
	float squaredMagnitude() const;

	void normalize();
	Vec2 normalizeCopy() const;

	void chamfer();

	float x;
	float y;

	static const Vec2 ZERO;
	static const Vec2 X_UNIT;
	static const Vec2 Y_UNIT;
	static const Vec2 NEGATIVE_Y_UNIT;
	static const Vec2 NEGATIVE_X_UNIT;
};

class Vec3 {
public:
	Vec3(void);
	Vec3(float xp, float yp, float zp);
	Vec3(const Vec3 &rhs);
	~Vec3(void);

	bool operator==(const Vec3 &rhs) const;
	bool operator!=(const Vec3 &rhs) const;
	void operator+=(const Vec3 &rhs);
	void operator-=(const Vec3 &rhs);

	Vec3& operator=(const Vec3 &rhs);
	Vec3& operator*=(const Vec3 &rhs);
	Vec3& operator*=(const float scalar);

	Vec3 operator+(const Vec3 &rhs) const;
	Vec3 operator-(const Vec3 &rhs) const;
	Vec3 operator*(const Vec3 &rhs) const;
	Vec3 operator/(const Vec3 &rhs) const;

	Vec3 operator+(const float scalar) const;
	Vec3 operator-(const float scalar) const;
	Vec3 operator*(const float scalar) const;
	Vec3 operator/(const float scalar) const;

	float dotProduct(const Vec3 &rhs) const;
	float magnitude() const;
	float squaredMagnitude() const;
	Vec3 cross(Vec3 v) const;

	void normalize();
	Vec3 normalizeCopy() const;

	void chamfer();

	float x;
	float y;
	float z;

	static const Vec3 ZERO;
	static const Vec3 X_UNIT;
	static const Vec3 Y_UNIT;
	static const Vec3 Z_UNIT;
	static const Vec3 NEGATIVE_Y_UNIT;
	static const Vec3 NEGATIVE_X_UNIT;
	static const Vec3 NEGATIVE_Z_UNIT;
};

