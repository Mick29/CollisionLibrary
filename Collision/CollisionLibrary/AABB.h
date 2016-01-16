#pragma once

#include "Vector.h"

class AABB {
public:
	AABB();
	AABB(const Vec3& min, const Vec3& max);

	bool intersect(AABB box);
	bool containsPoint(Vec3 point);

	Vec3 getMin();
	Vec3 getMax();
private:
	Vec3 mMin;
	Vec3 mMax;
};