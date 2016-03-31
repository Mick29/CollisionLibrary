#pragma once

#include "Vector.h"
#include "Physical.h"

class CollisionResult {
public:
	CollisionResult() {};
	~CollisionResult() {};

//	Vec3 getPosition();
//
//	GameObject* getCollisionObject();
//private:
	Vec3 mPosition;
	Vec3 mNormal;

	Physical* mCollisionObject;
};