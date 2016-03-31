#pragma once

#include "Physical.h"

enum OBJECT_TYPES {
	OBJECT_NONE = 0,
	OBJECT_PLAYER3D,
	OBJECT_PLAYER2D
};

class GameObject : public Physical {
public:
	GameObject() { this->setPosition(Vec3::ZERO); this->setBoundingBox(AABB(Vec3(0, 0, 0), Vec3(0, 0, 0))); };
	GameObject(const Vec3& pos, const AABB& bounds) { this->setPosition(pos); this->setBoundingBox(bounds); };
	~GameObject() {};

	virtual bool init() { return true; };

	virtual void update(unsigned int dt) {};

	virtual void render() {};

	virtual void handleCollision(Physical* other) {};

	int getType() { return mType; }

	GameObject* getObjectFromPhysical(Physical* other) { return static_cast<GameObject*>(other); }
protected:
	unsigned int mType;
};

class GameObject2D : public Physical2D {
public:
	GameObject2D() { this->setPosition(Vec2::ZERO); this->setBoundingBox(AABB2D(Vec2(0, 0), Vec2(0, 0))); };
	GameObject2D(const Vec2& pos, const AABB2D& bounds) { this->setPosition(pos); this->setBoundingBox(bounds); };
	~GameObject2D() {};

	virtual bool init() { return true; };

	virtual void update(unsigned int dt) {};

	virtual void render() {};

	virtual void handleCollision(Physical2D* other) {};

	int getType() { return mType; }

	GameObject2D* getObjectFromPhysical(Physical2D* other) { return static_cast<GameObject2D*>(other); }
protected:
	unsigned int mType;
};