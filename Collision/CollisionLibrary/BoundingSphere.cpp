#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() : mRadius(0.0f), mCentre(Vec3::ZERO) {}
BoundingSphere::BoundingSphere(float r, Vec3 c) : mRadius(r), mCentre(c) {}

BoundingSphere::~BoundingSphere() {}

bool BoundingSphere::containsPoint(Vec3 p) {
	float distance = mCentre.magnitude() - p.magnitude();

	if (distance < mRadius) {
		return true;
	}

	return false;
}

float BoundingSphere::getRadius() {
	return mRadius;
}

void BoundingSphere::setRadius(float r) {
	mRadius = r;
}

Vec3 BoundingSphere::getCentre() {
	return mCentre;
}

void BoundingSphere::setCentre(Vec3 c) {
	mCentre = c;
}