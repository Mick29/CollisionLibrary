#include "Vector.h"

#include <math.h>

const Vec2 Vec2::ZERO(0.0f, 0.0f);
const Vec2 Vec2::X_UNIT(1.0f, 0.0f);
const Vec2 Vec2::Y_UNIT(0.0f, 1.0f);
const Vec2 Vec2::NEGATIVE_X_UNIT(-1.0f, 0.0f);
const Vec2 Vec2::NEGATIVE_Y_UNIT(0.0f, -1.0f);

const Vec3 Vec3::ZERO(0.0f, 0.0f, 0.0f);
const Vec3 Vec3::X_UNIT(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::Y_UNIT(0.0f, 1.0f, 0.0f);
const Vec3 Vec3::Z_UNIT(0.0f, 0.0f, 1.0f);
const Vec3 Vec3::NEGATIVE_X_UNIT(-1.0f, 0.0f, 0.0f);
const Vec3 Vec3::NEGATIVE_Y_UNIT(0.0f, -1.0f, 0.0f);
const Vec3 Vec3::NEGATIVE_Z_UNIT(0.0f, 0.0f, -1.0f);

Vec2::Vec2(void) : x(0), y(0) {}

Vec2::Vec2(float xp, float yp) : x(xp), y(yp) {}

Vec2::~Vec2() {}

Vec2::Vec2(const Vec2 &rhs)
{
	x = rhs.x;
	y = rhs.y;
}

bool Vec2::operator==(const Vec2 &rhs) const
{
	if (x == rhs.x && y == rhs.y)
		return true;

	return false;
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
	if (x != rhs.x || y != rhs.y)
		return true;

	return false;
}

void Vec2::operator+=(const Vec2 &rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

Vec2& Vec2::operator=(const Vec2 &rhs)
{
	x = rhs.x;
	y = rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(const Vec2 &rhs)
{
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(const float scaler)
{
	x *= scaler;
	y *= scaler;

	return *this;
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
	Vec2 temp;
	temp.x = x + rhs.x;
	temp.y = y + rhs.y;

	return temp;
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
	Vec2 temp;
	temp.x = x - rhs.x;
	temp.y = y - rhs.y;

	return temp;
}

Vec2 Vec2::operator*(const Vec2 &rhs) const
{
	Vec2 temp;
	temp.x = x * rhs.x;
	temp.y = y * rhs.y;

	return temp;
}

Vec2 Vec2::operator/(const Vec2 &rhs) const
{
	Vec2 temp;
	temp.x = x / rhs.x;
	temp.y = y / rhs.y;

	return temp;
}

Vec2 Vec2::operator+(const float scalar) const
{
	Vec2 temp;
	temp.x = x + scalar;
	temp.y = y + scalar;

	return temp;
}

Vec2 Vec2::operator-(const float scalar) const
{
	Vec2 temp;
	temp.x = x - scalar;
	temp.y = y - scalar;

	return temp;
}

Vec2 Vec2::operator*(const float scalar) const
{
	Vec2 temp;
	temp.x = x * scalar;
	temp.y = y * scalar;

	return temp;
}

Vec2 Vec2::operator/(const float scalar) const
{
	Vec2 temp;
	temp.x = x / scalar;
	temp.y = y / scalar;

	return temp;
}

float Vec2::dotProduct(const Vec2 &rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}

float Vec2::magnitude() const
{
	return sqrtf((x * x) + (y * y));
}

float Vec2::squaredMagnitude() const
{
	return (x * x) + (y * y);
}

void Vec2::normalize()
{
	float length = 1.0f * magnitude();

	x *= length;
	y *= length;
}

Vec2 Vec2::normalizeCopy() const
{
	float length = 1.0f * magnitude();

	Vec2 temp;
	temp.x *= length;
	temp.y *= length;

	return temp;
}

void Vec2::chamfer()
{
	int ix = static_cast<int>(this->x);
	int iy = static_cast<int>(this->y);

	this->x = static_cast<float>(ix);
	this->y = static_cast<float>(iy);
}

//Vector3
Vec3::Vec3(void) : x(0), y(0) {}

Vec3::Vec3(float xp, float yp, float zp) : x(xp), y(yp), z(zp) {}

Vec3::~Vec3() {}

Vec3::Vec3(const Vec3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

bool Vec3::operator==(const Vec3 &rhs) const
{
	if (x == rhs.x && y == rhs.y && z == rhs.z)
		return true;

	return false;
}

bool Vec3::operator!=(const Vec3 &rhs) const
{
	if (x != rhs.x || y != rhs.y || z != rhs.z)
		return true;

	return false;
}

void Vec3::operator+=(const Vec3 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

void Vec3::operator-=(const Vec3 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

Vec3& Vec3::operator=(const Vec3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

Vec3& Vec3::operator*=(const Vec3 &rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;

	return *this;
}

Vec3& Vec3::operator*=(const float scaler)
{
	x *= scaler;
	y *= scaler;
	z *= scaler;

	return *this;
}

Vec3 Vec3::operator+(const Vec3 &rhs) const
{
	Vec3 temp;
	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;

	return temp;
}

Vec3 Vec3::operator-(const Vec3 &rhs) const
{
	Vec3 temp;
	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;

	return temp;
}

Vec3 Vec3::operator*(const Vec3 &rhs) const
{
	Vec3 temp;
	temp.x = x * rhs.x;
	temp.y = y * rhs.y;
	temp.z = z * rhs.z;

	return temp;
}

Vec3 Vec3::operator/(const Vec3 &rhs) const
{
	Vec3 temp;
	temp.x = x / rhs.x;
	temp.y = y / rhs.y;
	temp.z = z / rhs.z;

	return temp;
}

Vec3 Vec3::operator+(const float scalar) const
{
	Vec3 temp;
	temp.x = x + scalar;
	temp.y = y + scalar;
	temp.z = z + scalar;

	return temp;
}

Vec3 Vec3::operator-(const float scalar) const
{
	Vec3 temp;
	temp.x = x - scalar;
	temp.y = y - scalar;
	temp.z = z - scalar;

	return temp;
}

Vec3 Vec3::operator*(const float scalar) const
{
	Vec3 temp;
	temp.x = x * scalar;
	temp.y = y * scalar;
	temp.z = z * scalar;

	return temp;
}

Vec3 Vec3::operator/(const float scalar) const
{
	Vec3 temp;
	temp.x = x / scalar;
	temp.y = y / scalar;
	temp.z = z / scalar;

	return temp;
}

float Vec3::dotProduct(const Vec3 &rhs) const
{
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

float Vec3::magnitude() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vec3::squaredMagnitude() const
{
	return (x * x) + (y * y) + (z * z);
}

Vec3 Vec3::cross(Vec3 v) const
{
	Vec3 temp;
	temp.x = (y * v.z) - (v.y * z);
	temp.y = (z * v.x) - (v.z * x);
	temp.z = (x * v.y) - (v.x * y);

	return temp;
}

void Vec3::normalize()
{
	float length = 1.0f * magnitude();

	x *= length;
	y *= length;
	z *= length;
}

Vec3 Vec3::normalizeCopy() const
{
	float length = 1.0f * magnitude();

	Vec3 temp;
	temp.x *= length;
	temp.y *= length;
	temp.z *= length;

	return temp;
}

void Vec3::chamfer()
{
	int ix = static_cast<int>(this->x);
	int iy = static_cast<int>(this->y);
	int iz = static_cast<int>(this->z);

	this->x = static_cast<float>(ix);
	this->y = static_cast<float>(iy);
	this->z = static_cast<float>(iz);
}