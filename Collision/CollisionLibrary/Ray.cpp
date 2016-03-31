#include "Ray.h"

Ray::Ray(Vec3 o, Vec3 d) : mOrigin(o), mDirection(d) {
	mInverseDirection = Vec3(1 / mDirection.x, 1 / mDirection.y, 1 / mDirection.z);

	mSigns[0] = (mInverseDirection.x < 0);
	mSigns[1] = (mInverseDirection.y < 0);
	mSigns[2] = (mInverseDirection.z < 0);
}

Ray::~Ray() {}

Vec3 Ray::getOrigin() {
	return mOrigin;
}

Vec3 Ray::getDirection() {
	return mDirection;
}

void Ray::setOrigin(Vec3 o) {
	mOrigin = o;
}

void Ray::setDirection(Vec3 d) {
	mDirection = d;
}

Vec3 Ray::getInverseDirection() {
	return mInverseDirection;
}

int Ray::getSignX() {
	return mSigns[0];
}

int Ray::getSignY() {
	return mSigns[1];
}

int Ray::getSignZ() {
	return mSigns[2];
}