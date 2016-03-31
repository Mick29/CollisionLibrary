#pragma once

#include "Vector.h"

class BoundingSphere {
public:
	BoundingSphere();
	BoundingSphere(float r, Vec3 c);

	~BoundingSphere();

	bool containsPoint(Vec3 p);

	float getRadius();
	void setRadius(float r);

	Vec3 getCentre();
	void setCentre(Vec3 c);
private:
	float mRadius;
	Vec3 mCentre;
};