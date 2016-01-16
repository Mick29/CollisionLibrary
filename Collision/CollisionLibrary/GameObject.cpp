#include "GameObject.h"

GameObject::GameObject() : mPosition(Vec3::ZERO), mBoundingBox(AABB(Vec3::ZERO, Vec3::ZERO)), mHasBounds(false) {}
GameObject::GameObject(Vec3 pos, AABB box) : mPosition(pos), mBoundingBox(box), mHasBounds(true) {}
GameObject::~GameObject() {}

Vec3 GameObject::getPosition() {
	return mPosition;
}

void GameObject::setPosition(Vec3 pos) {
	mPosition = pos;
}

AABB GameObject::getBoundingBox() {
	return mBoundingBox;
}

void GameObject::setBoundingBox(AABB box) {
	mBoundingBox = box;
	mHasBounds = true;
}

bool GameObject::update(unsigned int dt) {
	return false;
}