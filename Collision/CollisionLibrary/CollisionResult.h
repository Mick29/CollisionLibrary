#pragma once

#include "Vector.h"
#include "Physical.h"
//#include "Ray.h"

class CollisionResult {
public:
	CollisionResult();
	CollisionResult(Physical* object, Physical* hitObject);
	~CollisionResult();

	Vec3 mPosition;
	Vec3 mNormal;
	//Ray mRay;

	Physical* mCollisionObject;
	Physical* mOther;
};

class CollisionResult2D {
public:
	CollisionResult2D();
	CollisionResult2D(Physical2D* object, Physical2D* hitObject);
	~CollisionResult2D();

	Vec2 mPosition;
	Vec2 mNormal;
	//Ray mRay;

	Physical2D* mCollisionObject;
	Physical2D* mOther;
};