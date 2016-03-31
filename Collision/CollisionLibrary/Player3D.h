#pragma once

#include "GameObject.h"

class Player3D : public GameObject {
public:
	Player3D();
	Player3D(Vec3 pos, AABB bounds);
	~Player3D();

	bool init();

	void update(unsigned int dt);

	void render();

	void handleCollision(Physical* Other);

	void handleInput(unsigned char key, int x, int y);
	void handleInputUp(unsigned char key, int x, int y);
	void handleInputSpecial(int key, int xx, int yy);
private:
	Vec3 mVelocity;
	Vec3 mDirection;
	Vec3 mPreviousPosition;

	bool mOnGround;
};