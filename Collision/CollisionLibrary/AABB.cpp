#pragma once

#include "AABB.h"
#include "Ray.h"
#include "CollisionResult.h"
#include <gl\freeglut.h>

float squared(float i) {
	return i * i;

}
AABB::AABB() : mMin(Vec3(0, 0, 0)), mMax(Vec3(0, 0, 0)), mRayLength(0) {}
AABB::AABB(const Vec3& min, const Vec3& max) : mMin(min), mMax(max), mRayLength(0) {}

bool AABB::intersect(AABB box) {
	//CollisionResult newResult;

	return (mMax.x > box.getMin().x &&
		mMin.x < box.getMax().x &&
		mMax.y > box.getMin().y &&
		mMin.y < box.getMax().y &&
		mMax.z > box.getMin().z &&
		mMin.z < box.getMax().z);
}

bool AABB::intersect(Ray r) {
	float tMin, tMax, tYMin, tYMax, tZMin, tZMax;
	
	tMin = (mMin.x - r.getOrigin().x) * r.getInverseDirection().x;
	tMax = (mMax.x - r.getOrigin().x) * r.getInverseDirection().x;

	tYMin = (mMin.y - r.getOrigin().y) * r.getInverseDirection().y;
	tYMax = (mMax.y - r.getOrigin().y) * r.getInverseDirection().y;

	if ((tMax > tYMax) || (tYMin > tMax)) {
		mRayLength = tMin;
		return false;
	}
	if (tYMin > tMin) {
		tMin = tYMin;
	}
	if (tYMax < tMax) {
		tMax = tYMax;
	}

	tZMin = (mMin.z - r.getOrigin().z) * r.getInverseDirection().z;
	tZMax = (mMax.z - r.getOrigin().z) * r.getInverseDirection().z;

	if ((tMin > tZMax) || tZMin > tMax) {
		mRayLength = tMin;
		return false;
	}
	if (tZMin > tZMin) {
		tMin = tZMin;
	}
	if (tZMax < tMax) {
		tMax = tZMax;
	}

	//return ((tMin < i1) && (tMax > i0));
	setRayLength(tMax);

	return true;
}

bool AABB::intersect(BoundingSphere s) {
	float dist_squared = s.getRadius() * s.getRadius();
	
	if (s.getCentre().x < mMin.x) dist_squared -= squared(s.getCentre().x - mMin.x);
	else if (s.getCentre().x > mMax.y) dist_squared -= squared(s.getCentre().x - mMax.x);
	if (s.getCentre().y < mMin.y) dist_squared -= squared(s.getCentre().y - mMin.y);
	else if (s.getCentre().y > mMax.y) dist_squared -= squared(s.getCentre().y - mMax.y);
	if (s.getCentre().z < mMin.z) dist_squared -= squared(s.getCentre().z - mMin.z);
	else if (s.getCentre().z > mMax.y) dist_squared -= squared(s.getCentre().z - mMax.z);

	return dist_squared > 0;
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

float AABB::getRayLength() {
	return mRayLength;
}

void AABB::setRayLength(float l) {
	mRayLength = l;
}

void AABB::renderWireframe() {
	Vec3 verts[8];
	verts[0] = getMin();
	verts[1] = Vec3(getMin().x, getMin().y, getMax().z);
	verts[2] = Vec3(getMin().x, getMax().y, getMin().z);
	verts[3] = Vec3(getMax().x, getMin().y, getMin().z);

	verts[7] = getMax();
	verts[4] = Vec3(getMax().x, getMax().y, getMin().z);
	verts[5] = Vec3(getMax().x, getMin().y, getMax().z);
	verts[6] = Vec3(getMin().x, getMax().y, getMax().z);

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	glVertex3f(verts[0].x, verts[0].y, verts[0].z);
	glVertex3f(verts[1].x, verts[1].y, verts[1].z);

	glVertex3f(verts[0].x, verts[0].y, verts[0].z);
	glVertex3f(verts[2].x, verts[2].y, verts[2].z);

	glVertex3f(verts[0].x, verts[0].y, verts[0].z);
	glVertex3f(verts[3].x, verts[3].y, verts[3].z);

	glVertex3f(verts[7].x, verts[7].y, verts[7].z);
	glVertex3f(verts[4].x, verts[4].y, verts[4].z);

	glVertex3f(verts[7].x, verts[7].y, verts[7].z);
	glVertex3f(verts[5].x, verts[5].y, verts[5].z);

	glVertex3f(verts[7].x, verts[7].y, verts[7].z);
	glVertex3f(verts[6].x, verts[6].y, verts[6].z);

	glVertex3f(verts[1].x, verts[1].y, verts[1].z);
	glVertex3f(verts[6].x, verts[6].y, verts[6].z);

	glVertex3f(verts[1].x, verts[1].y, verts[1].z);
	glVertex3f(verts[5].x, verts[5].y, verts[5].z);

	glVertex3f(verts[4].x, verts[4].y, verts[4].z);
	glVertex3f(verts[2].x, verts[2].y, verts[2].z);

	glVertex3f(verts[4].x, verts[4].y, verts[4].z);
	glVertex3f(verts[3].x, verts[3].y, verts[3].z);

	glVertex3f(verts[2].x, verts[2].y, verts[2].z);
	glVertex3f(verts[6].x, verts[6].y, verts[6].z);

	glVertex3f(verts[3].x, verts[3].y, verts[3].z);
	glVertex3f(verts[5].x, verts[5].y, verts[5].z);

	glEnd();
}

AABB2D::AABB2D() {
	mMin = Vec2::ZERO;
	mMax = Vec2::ZERO;
}

AABB2D::AABB2D(const Vec2& min, const Vec2& max) {
	mMin = min;
	mMax = max;
}

bool AABB2D::intersect(AABB2D box) {
	return (mMax.x > box.mMin.x &&
		mMin.x < box.mMax.x &&
		mMax.y > box.mMin.y &&
		mMin.y < box.mMax.y);
}

void AABB2D::render() {
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, mMax.y);

		glVertex2f(0, mMax.y);
		glVertex2f(mMax.x, mMax.y);

		glVertex2f(mMax.x, mMax.y);
		glVertex2f(mMax.x, 0);

		glVertex2f(mMax.x, 0);
		glVertex2f(0, 0);
	glEnd();
}