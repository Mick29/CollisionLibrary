#pragma once

#include "Vector.h"

class Ray {
public:
	Ray(Vec3 o, Vec3 d);
	~Ray();

	Vec3 getOrigin();
	Vec3 getDirection();

	void setOrigin(Vec3 o);
	void setDirection(Vec3 d);

	Vec3 getInverseDirection();

	int getSignX();
	int getSignY();
	int getSignZ();
private:
	Vec3 mOrigin;
	Vec3 mDirection;
	Vec3 mInverseDirection;

	int mSigns[3];
};