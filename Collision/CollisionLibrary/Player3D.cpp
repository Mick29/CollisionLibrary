#include "Player3D.h"
#include "LOpenGL.h"

Player3D::Player3D() : mVelocity(Vec3::ZERO), GameObject() {}
Player3D::Player3D(Vec3 pos, AABB bounds) : GameObject(pos, bounds) {}
Player3D::~Player3D() {}

bool Player3D::init() {
	if (!GameObject::init()) {
		return false;
	}

	mType = OBJECT_PLAYER3D;
	mStatic = false;
	mBoundingBox.mMax = Vec3(50, 50, 50);
	mOnGround = false;

	return true;
}

void Player3D::update(unsigned int dt) {
	Physical::update(dt);

	mPreviousPosition = mPosition;

	mHasMoved = false;

	if (mDirection.z == 1) {
		if (mVelocity.z >= 50) {
			mVelocity.z = 50.0f;
		}
	}
	else {
		if (mVelocity.z <= -50) {
			mVelocity.z = -50.0f;
		}
	}

	if (mDirection.x == 1) {
		if (mVelocity.x >= 50) {
			mVelocity.x = 50.0f;
		}
	}
	else {
		if (mVelocity.x <= -50) {
			mVelocity.x = -50.0f;
		}
	}
	
	//Get position before gravity for checking if moved
	Vec3 tempPos = mPosition + mVelocity;

	mVelocity.y += GRAVITY_SPEED;

	mPosition += mVelocity;

	if(tempPos != mPreviousPosition)
		mHasMoved = true;
}

void Player3D::render() {
	glPushMatrix();

	glColor3f(0x00, 0x11, 0xaa);

	this->getBoundingBox().renderSolid();

	glPopMatrix();
}

void Player3D::handleCollision(Physical* other) {
	//Convert the other into a GameObject
   	GameObject* object = this->getObjectFromPhysical(other);

	AABB temp = mBoundingBox;
	temp.mMax += mPosition;
	temp.mMin += mPosition;

	while (mPosition.y < other->getBoundingBox().mMax.y) {
		mPosition.y += 1;
		temp.mMax += mPosition;
		temp.mMin += mPosition;
	}

	this->mVelocity.y = 0.0f;
	this->mOnGround = true;
}

void Player3D::handleInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		mVelocity.z -= 10.0f;
		mDirection.z = -1;
		break;
	case 's':
		mVelocity.z += 10.0f;
		mDirection.z = 1;
		break;
	case 'a':
		mVelocity.x -= 10.0f;
		mDirection.x = -1;
		break;
	case 'd':
		mVelocity.x += 10.0f;
		mDirection.x = 1;
		break;
	case ' ':
		mVelocity.y += 50.0f;
		mDirection.y = 1;
		mOnGround = false;
		break;
	}
}

void Player3D::handleInputUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		mVelocity.z = 0.0f;
		break;
	case 's':
		mVelocity.z = 0.0f;
		break;
	case 'a':
		mVelocity.x = 0.0f;
		break;
	case 'd':
		mVelocity.x = 0.0f;
		break;
	}
}

void Player3D::handleInputSpecial(int key, int xx, int yy) {
	switch (key) {

	}
}