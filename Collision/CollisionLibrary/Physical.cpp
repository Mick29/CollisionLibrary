#include "Physical.h"

Physical::Physical() : mPosition(Vec3::ZERO), mBoundingBox(AABB(Vec3::ZERO, Vec3::ZERO)), mHasBounds(false), mAlive(true), timer(50), goRight(true), mHasMoved(false) {}
Physical::Physical(const Vec3& pos,const AABB& box) : mPosition(pos), mBoundingBox(box), mHasBounds(true), mAlive(true), timer(50), goRight(true), mHasMoved(false), mBoundingSphere(BoundingSphere(0, Vec3::ZERO)) {}
Physical::Physical(const Vec3& pos, const BoundingSphere& sphere) : mPosition(pos), mBoundingBox(AABB(Vec3::ZERO, Vec3::ZERO)), mHasBounds(true), mAlive(true), timer(50), goRight(true), mHasMoved(false), mBoundingSphere(sphere) {}
Physical::~Physical() {}

Vec3 Physical::getPosition() {
	return mPosition;
}

void Physical::setPosition(Vec3 pos) {
	mPosition = pos;
}

AABB Physical::getBoundingBox(bool pos) {
	AABB temp = mBoundingBox;

	if (pos) {
		temp.mMin += mPosition;
		temp.mMax += mPosition;
	}

	return temp;
}

void Physical::setBoundingBox(AABB box) {
	mBoundingBox = box;
	mHasBounds = true;
}

void Physical::update(unsigned int dt) {
	if (id % 5 == 0) {
		if (timer == 0) {
			if (goRight)
				goRight = false;
			else
				goRight = true;
			timer = 50;
		}

		if (goRight)
			mPosition.x += 10.0f * dt;
		else
			mPosition.x -= 10.0f * dt;

		timer--;

		mHasMoved = true;
	}
	else {
		mHasMoved = false;
	}
}

bool Physical::isAlive() {
	return mAlive;
}

bool Physical::hasMoved() {
	return mHasMoved;
}

BoundingSphere Physical::getBoundingSphere() {
	return mBoundingSphere;
}

void Physical::setBoundingSphere(BoundingSphere sphere) {
	mBoundingSphere = sphere;
}

Physical2D::Physical2D() : mPosition(Vec2::ZERO), mBoundingBox(AABB2D(Vec2::ZERO, Vec2::ZERO)), mHasBounds(false), mAlive(true), timer(50), goRight(true), mHasMoved(false) {}
Physical2D::Physical2D(const Vec2& pos,const AABB2D& box) : mPosition(pos), mBoundingBox(box), mHasBounds(true), mAlive(true), timer(50), goRight(true), mHasMoved(false) {}
//Physical2D::Physical2D(const Vec2& pos, const BoundingSphere& sphere) : mPosition(pos), mBoundingBox(AABB(Vec3::ZERO, Vec3::ZERO)), mHasBounds(true), mAlive(true), timer(50000), goRight(true), mHasMoved(false), mBoundingSphere(sphere) {}
Physical2D::~Physical2D() {}

Vec2 Physical2D::getPosition() {
	return mPosition;
}

void Physical2D::setPosition(Vec2 pos) {
	mPosition = pos;
}

AABB2D Physical2D::getBoundingBox(bool pos) {
	AABB2D temp = mBoundingBox;

	if (pos) {
		temp.mMin += mPosition;
		temp.mMax += mPosition;
	}

	return temp;
}

void Physical2D::setBoundingBox(AABB2D box) {
	mBoundingBox = box;
	mHasBounds = true;
}

void Physical2D::update(unsigned int dt) {
	//if (id % 5 == 0) {
	//	if (timer == 0) {
	//		if (goRight)
	//			goRight = false;
	//		else
	//			goRight = true;
	//		timer = 50;
	//	}

	//	if (goRight)
	//		mPosition.x += 1.0f * dt;
	//	else
	//		mPosition.x -= 1.0f * dt;

	//	timer--;

	//	mHasMoved = true;
	//}
	//else {
	//	mHasMoved = false;
	//}
}

bool Physical2D::isAlive() {
	return mAlive;
}

bool Physical2D::hasMoved() {
	return mHasMoved;
}