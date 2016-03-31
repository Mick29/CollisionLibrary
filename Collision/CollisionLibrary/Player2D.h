#pragma once

#include "GameObject.h"

class Player2D : public GameObject2D {
public:
	Player2D();
	Player2D(Vec2 pos, AABB2D bounds);
	~Player2D();

	bool init();

	void update(unsigned int dt);

	void render();

	void handleCollision(Physical2D* Other);

	void handleInput(unsigned char key, int x, int y);
	void handleInputUp(unsigned char key, int x, int y);
	void handleInputSpecial(int key, int xx, int yy);
private:
	Vec2 mVelocity;
	Vec2 mDirection;

	bool mOnGround;
};