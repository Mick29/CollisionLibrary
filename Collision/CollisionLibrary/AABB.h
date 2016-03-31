#pragma once

#include "Vector.h"

class Ray;
class BoundingSphere;
class CollisionResult;
class AABB {
public:
	AABB();
	AABB(const Vec3& min, const Vec3& max);

	bool intersect(AABB box);
	bool intersect(Ray r);
	bool intersect(BoundingSphere s);
	bool containsPoint(Vec3 point);

	Vec3 getMin();
	Vec3 getMax();

	Vec3 mMin;
	Vec3 mMax;

	float getRayLength();
	void setRayLength(float l);

	void renderWireframe();
	void renderSolid();
private:
	float mRayLength;
	//BoundingSphere mBoundingSphere;
};

class AABB2D {
public:
	AABB2D();
	AABB2D(const Vec2& min, const Vec2& max);

	bool intersect(AABB2D box);

	Vec2 mMin;
	Vec2 mMax;

	void render();
};