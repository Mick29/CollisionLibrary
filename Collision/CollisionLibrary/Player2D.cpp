#include "Player2D.h"
#include "LOpenGL.h"
#include "LUtil.h"

Player2D::Player2D() : mVelocity(Vec2::ZERO), GameObject2D() {}
Player2D::Player2D(Vec2 pos, AABB2D bounds) : GameObject2D(pos, bounds) {}
Player2D::~Player2D() {}

bool Player2D::init() {
	if (!GameObject2D::init()) {
		return false;
	}

	mType = OBJECT_PLAYER2D;
	mStatic = false;
	mBoundingBox.mMin = Vec2(0, 0);
	mBoundingBox.mMax = Vec2(50, 50);
	mOnGround = false;

	return true;
}

void Player2D::update(unsigned int dt) {
	Physical2D::update(dt);

	mHasMoved = false;

	if (mDirection.x == 1) {
		if (mVelocity.x >= 5) {
			mVelocity.x = 5.0f;
		}
	}
	else if(mDirection.x == -1){
		if (mVelocity.x <= -5) {
			mVelocity.x = -5.0f;
		}
	}
	else {
		mVelocity.x = 0.0f;
	}

	mVelocity.y += 0.4;

	mPosition += mVelocity;
	mHasMoved = true;
}

void Player2D::render() {
	glPushMatrix();

	glColor3f(0x00, 0x11, 0xaa);

	glTranslatef(this->getPosition().x + gCameraX, this->getPosition().y + gCameraY, 0.0f);
	AABB2D temp = this->getBoundingBox(false);
	temp.render();

	glPopMatrix();
}

void Player2D::handleCollision(Physical2D* other) {
	//Convert the other into a GameObject2D
	GameObject2D* object = this->getObjectFromPhysical(other);

	AABB2D temp = mBoundingBox;
	temp.mMax += mPosition;
	temp.mMin += mPosition;

	while (mPosition.y + 50 > other->getBoundingBox().mMin.y) {
		mPosition.y -= 1;
		temp.mMax += mPosition;
		temp.mMin += mPosition;
	}

	this->mVelocity.y = 0.0f;
	this->mOnGround = true;

	other->mCollisionDetected = true;
}

void Player2D::handleInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		break;
	case 's':
		break;
	case 'a':
		mVelocity.x -= 5.0f;
		mDirection.x = -1;
		break;
	case 'd':
		mVelocity.x += 5.0f;
		mDirection.x = 1;
		break;
	case ' ':
		mVelocity.y -= 10.0f;
		mDirection.y = 1;
		mOnGround = false;
		break;
	}
}

void Player2D::handleInputUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		break;
	case 's':
		break;
	case 'a':
		mDirection.x = 0;
		break;
	case 'd':
		mDirection.x = 0;
		break;
	}
}

void Player2D::handleInputSpecial(int key, int xx, int yy) {
	switch (key) {

	}
}