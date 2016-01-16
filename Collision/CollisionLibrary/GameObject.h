#pragma once

#include "Vector.h"
#include "AABB.h"

class GameObject {
public:
	GameObject();
	GameObject(Vec3 pos, AABB box);
	~GameObject();

	Vec3 getPosition();
	void setPosition(Vec3 pos);

	AABB getBoundingBox();
	void setBoundingBox(AABB box);

	bool mHasBounds;

	bool update(unsigned int dt);
private:
	Vec3 mPosition;
	AABB mBoundingBox;
};