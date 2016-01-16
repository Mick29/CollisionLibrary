#pragma once

#include "AABB.h"

AABB::AABB(): mMin(Vec3(0, 0, 0)), mMax(Vec3(0, 0, 0)) {}
AABB::AABB(const Vec3& min, const Vec3& max) : mMin(min), mMax(max) {}

bool AABB::intersect(AABB box) {
	return (mMax.x > box.getMin().x &&
		mMin.x < box.getMax().x &&
		mMax.y > box.getMin().y &&
		mMin.y < box.getMax().y &&
		mMax.z > box.getMin().z &&
		mMin.z < box.getMax().z);
}

bool AABB::containsPoint(Vec3 point) {
	return(point.x > mMin.x && point.x < mMax.x &&
		point.y > mMin.y && point.y < mMax.y &&
		point.z > mMin.z && point.z < mMax.z);
}

Vec3 AABB::getMin() {
	return mMin;
}

Vec3 AABB::getMax() {
	return mMax;
}