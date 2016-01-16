#include "Octree.h"
#include "GLShapes.h"
#include <algorithm>

bool Octree::mReady = false;
bool Octree::mBuilt = false;
std::queue<GameObject*> Octree::mPendingInsertion;

Octree::Octree(AABB bounding, std::vector<GameObject*> objects) {
	mBounds = bounding;

	mObjects = objects;

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;
}

Octree::Octree(AABB bounding) {
	mBounds = bounding;

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;
}

Octree::Octree() {
	mBounds = AABB(Vec3::ZERO, Vec3::ZERO);

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;
}

Octree::~Octree() {
	for (int i = 0; i < 8; i++)
		delete mNodes[i];
}

void Octree::renderTree() {
	Vec3 verts[8];
	verts[0] = mBounds.getMin();
	verts[1] = Vec3(mBounds.getMin().x, mBounds.getMin().y, mBounds.getMax().z);
	verts[2] = Vec3(mBounds.getMin().x, mBounds.getMax().y, mBounds.getMin().z);
	verts[3] = Vec3(mBounds.getMax().x, mBounds.getMin().y, mBounds.getMin().z);

	verts[7] = mBounds.getMax();
	verts[4] = Vec3(mBounds.getMax().x, mBounds.getMax().y, mBounds.getMin().z);
	verts[5] = Vec3(mBounds.getMax().x, mBounds.getMin().y, mBounds.getMax().z);
	verts[6] = Vec3(mBounds.getMin().x, mBounds.getMax().y, mBounds.getMax().z);

	glLineWidth(1.0f);
	glColor3f(0x00, 0xFF, 0x00);

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

	for (int i = 0; i < 8; i++) {
		if (mNodes[i] != NULL) {
			mNodes[i]->renderTree();
		}
	}
}

void Octree::buildTree() {
	if (mObjects.size() <= 1)
		return;

	Vec3 dim = mBounds.getMax() - mBounds.getMin();

	if (dim == Vec3::ZERO) {
		dim = mBounds.getMax() - mBounds.getMin();
	}

	if (dim.x <= MIN_SIZE && dim.y <= MIN_SIZE && dim.z <= MIN_SIZE)
		return;

	Vec3 halfDim = dim / 2.0f;
	Vec3 centre = mBounds.getMin() + halfDim;

	AABB octants[8];
	//Font-bottom-left
	octants[0] = AABB(mBounds.getMin(), centre);
	//Font-bottom-right
	octants[1] = AABB(Vec3(centre.x, mBounds.getMin().y, mBounds.getMin().z), Vec3(mBounds.getMax().x, centre.y, centre.z));
	//Back-bottom-right
	octants[2] = AABB(Vec3(centre.x, mBounds.getMin().y, centre.z), Vec3(mBounds.getMax().x, centre.y, mBounds.getMax().z));
	//Back-bottom-left
	octants[3] = AABB(Vec3(mBounds.getMin().x, mBounds.getMin().y, centre.z), Vec3(centre.x, centre.y, mBounds.getMax().z));
	//Front-top-left
	octants[4] = AABB(Vec3(mBounds.getMin().x, centre.y, mBounds.getMin().z), Vec3(centre.x, mBounds.getMax().y, centre.z));
	//Front-top-right
	octants[5] = AABB(Vec3(centre.x, centre.y, mBounds.getMin().z), Vec3(mBounds.getMax().x, mBounds.getMax().y, centre.z));
	//Back-top-right
	octants[6] = AABB(centre, mBounds.getMax());
	//Back-top-left
	octants[7] = AABB(Vec3(mBounds.getMin().x, centre.y, centre.z), Vec3(centre.x, mBounds.getMax().y, mBounds.getMax().z));

	//Stores objects for each octant
	std::vector<GameObject*> octList[8];

	//For objects that get moved down the tree
	std::vector<GameObject*> delist;

	for (GameObject* obj : mObjects) {
		if (obj->getBoundingBox().getMin() != obj->getBoundingBox().getMax()) {
			for (int i = 0; i < 8; i++) {
				if (octants[i].intersect(obj->getBoundingBox())) {
					octList[i].push_back(obj);
					delist.push_back(obj);
					break;
				}
			}
		}
	}

	for (int i = 0; i < delist.size(); i++) {
		mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), delist[i]), mObjects.end());
	}

	for (int i = 0; i < 8; i++) {
		if (octList[i].size() != 0) {
			mNodes[i] = createNode(octants[i], octList[i]);
			mActiveNodes |= (int8_t)(1 << i);
			mNodes[i]->buildTree();
		}
	}
}

Octree* Octree::createNode(AABB region, std::vector<GameObject*> objects) {
	if (objects.size() == 0)
		return NULL;

	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

Octree* Octree::createNode(AABB region, GameObject* object) {
	std::vector<GameObject*> objects;
	objects.push_back(object);

	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

void Octree::add(std::vector<GameObject*> objects) {
	for (GameObject* obj : objects) {
		if (obj->mHasBounds) {
			Octree::mPendingInsertion.push(obj);
			mReady = true;
		}
	}
}

void Octree::add(GameObject* object) {
	if (object->mHasBounds) {
		Octree::mPendingInsertion.push(object);
		mReady = true;
	}
}

void Octree::updateTree(unsigned int dt) {
	if (mBuilt) {
		if (mObjects.size() == 0) {
			if (mActiveNodes == 0) {
				if (mCurLife == -1) {
					mCurLife = mMaxLifespan;
				}
				else if (mCurLife > 0) {
					mCurLife--;
				}
			}
		}
		else {
			if (mCurLife != -1) {
				if (mMaxLifespan <= 64)
					mMaxLifespan *= 2;
				mCurLife = -1;
			}
		}

		std::vector<GameObject*> movedObjects;

		for (GameObject* obj : mObjects) {
			if (obj->update(dt)) {
				movedObjects.push_back(obj);
			}
		}

		//int size = mObjects.size();
		//for (int i = 0; i < size; i++) {

		//}

		//Update active nodes
		int index = 0;
		for (int flags = mActiveNodes; flags > 0; flags >>= 1, index++) {
			if ((flags & 1) == 1) mNodes[index]->updateTree(dt);
		}

		//Moved objects code
		index = 0;
		for (int flags = mActiveNodes; flags > 0; flags >>= 1, index++) {
			if ((flags & 1) == 1 && mNodes[index]->getCurrentLife()) {
				delete mNodes[index];
				mNodes[index] = NULL;
				mActiveNodes ^= (int8_t)(1 << index);
			}
		}

		if (mParent == NULL) {
			//Check Collisions
		}
	}
}

void Octree::updateTree() {
	if (!mBuilt) {

	}
}

void Octree::insertObject(GameObject* obj) {
	if (mObjects.size() <= 1 && mActiveNodes == 0) {
		mObjects.push_back(obj);
		return;
	}

	Vec3 dim = mBounds.getMax() - mBounds.getMin();

	if (dim.x <= MIN_SIZE && dim.y <= MIN_SIZE && dim.z <= MIN_SIZE) {
		mObjects.push_back(obj);
		return;
	}

	Vec3 halfDim = dim / 2.0f;
	Vec3 centre = mBounds.getMin() + halfDim;

	AABB octants[8];
	//Font-bottom-left
	octants[0] = (mNodes[0] != NULL) ? mNodes[0]->mBounds : AABB(mBounds.getMin(), centre);
	//Font-bottom-right
	octants[1] = (mNodes[1] != NULL) ? mNodes[1]->mBounds : AABB(Vec3(centre.x, mBounds.getMin().y, mBounds.getMin().z), Vec3(mBounds.getMax().x, centre.y, centre.z));
	//Back-bottom-right
	octants[2] = (mNodes[2] != NULL) ? mNodes[2]->mBounds : AABB(Vec3(centre.x, mBounds.getMin().y, centre.z), Vec3(mBounds.getMax().x, centre.y, mBounds.getMax().z));
	//Back-bottom-left
	octants[3] = (mNodes[3] != NULL) ? mNodes[3]->mBounds : AABB(Vec3(mBounds.getMin().x, mBounds.getMin().y, centre.z), Vec3(centre.x, centre.y, mBounds.getMax().z));
	//Front-top-left
	octants[4] = (mNodes[4] != NULL) ? mNodes[4]->mBounds : AABB(Vec3(mBounds.getMin().x, centre.y, mBounds.getMin().z), Vec3(centre.x, mBounds.getMax().y, centre.z));
	//Front-top-right
	octants[5] = (mNodes[5] != NULL) ? mNodes[5]->mBounds : AABB(Vec3(centre.x, centre.y, mBounds.getMin().z), Vec3(mBounds.getMax().x, mBounds.getMax().y, centre.z));
	//Back-top-right
	octants[6] = (mNodes[6] != NULL) ? mNodes[6]->mBounds : AABB(centre, mBounds.getMax());
	//Back-top-left
	octants[7] = (mNodes[7] != NULL) ? mNodes[7]->mBounds : AABB(Vec3(mBounds.getMin().x, centre.y, centre.z), Vec3(centre.x, mBounds.getMax().y, mBounds.getMax().z));

	if (obj->getBoundingBox().getMin() != obj->getBoundingBox().getMax() && mBounds.intersect(obj->getBoundingBox())) {
		bool found = false;

		for (int i = 0; i < 8; i++) {
			if (octants[i].intersect(obj->getBoundingBox())) {
				if (mNodes[i] != NULL) {
					mNodes[i]->insertObject(obj);
				}
				else {
					mNodes[i] = createNode(octants[i], obj);
					mActiveNodes |= (int8_t)(1 << i);
				}

				found = true;
			}
		}

		if (!found) mObjects.push_back(obj);
	}
	else {
		buildTree();
	}
}

int Octree::getCurrentLife() {
	return mCurLife;
}

Octree* Octree::getParent() {
	return mParent;
}