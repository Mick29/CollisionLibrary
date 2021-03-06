#pragma once

#define GRAVITY_SPEED 0.2

#include "Vector.h"
#include "AABB.h"
#include "BoundingSphere.h"

class Physical {
public:
	Physical();
	Physical(const Vec3& pos, const AABB& box);
	Physical(const Vec3& pos, const BoundingSphere& sphere);
	~Physical();

	Vec3 getPosition();
	void setPosition(Vec3 pos);

	AABB getBoundingBox(bool pos = true);
	void setBoundingBox(AABB box);

	BoundingSphere getBoundingSphere();
	void setBoundingSphere(BoundingSphere sphere);

	virtual void handleCollision(Physical* other) { mCollisionDetected = true; };

	bool mHasBounds;

	virtual void update(unsigned int dt);

	bool isAlive();
	bool hasMoved();
	bool mHasMoved;

	int id;
	bool mAlive;
	bool mStatic;
	bool mCollisionDetected;
protected:
	Vec3 mPosition;
	AABB mBoundingBox;
	BoundingSphere mBoundingSphere;

	int timer;
	bool goRight;
};

class Physical2D {
public:
	Physical2D();
	Physical2D(const Vec2& pos, const AABB2D& box);
	//Physical2D(const Vec2& pos, const BoundingSphere& sphere);
	~Physical2D();

	Vec2 getPosition();
	void setPosition(Vec2 pos);

	AABB2D getBoundingBox(bool pos = true);
	void setBoundingBox(AABB2D box);

	//BoundingSphere getBoundingSphere();
	//void setBoundingSphere(BoundingSphere sphere);

	virtual void handleCollision(Physical2D* other) { mCollisionDetected = true; };

	bool mHasBounds;

	virtual void update(unsigned int dt);

	bool isAlive();
	bool hasMoved();
	bool mHasMoved;

	int id;
	bool mAlive;
	bool mStatic;
	bool mCollisionDetected;
protected:
	Vec2 mPosition;
	AABB2D mBoundingBox;
	//BoundingSphere mBoundingSphere;

	int timer;
	bool goRight;
};