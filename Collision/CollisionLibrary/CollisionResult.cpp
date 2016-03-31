#include "CollisionResult.h"

CollisionResult::CollisionResult() {
	mPosition = Vec3::ZERO;
	mNormal = Vec3::ZERO;

	mCollisionObject = nullptr;
	mOther = nullptr;
}

CollisionResult::CollisionResult(Physical* object, Physical* hitObject) {
	mPosition = Vec3::ZERO;
	mNormal = Vec3::ZERO;

	mCollisionObject = object;
	mOther = hitObject;
}

CollisionResult::~CollisionResult() {}

CollisionResult2D::CollisionResult2D() {
	mPosition = Vec2::ZERO;
	mNormal = Vec2::ZERO;

	mCollisionObject = nullptr;
	mOther = nullptr;
}

CollisionResult2D::CollisionResult2D(Physical2D* object, Physical2D* hitObject) {
	mPosition = Vec2::ZERO;
	mNormal = Vec2::ZERO;

	mCollisionObject = object;
	mOther = hitObject;
}

CollisionResult2D::~CollisionResult2D() {}