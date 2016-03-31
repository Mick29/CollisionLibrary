#include "Octree.h"
#include "GLShapes.h"
#include <algorithm>
#include <math.h>

bool Octree::mReady = false;
bool Octree::mBuilt = false;
std::vector<Physical*> Octree::mPendingInsertion;

Octree::Octree(AABB bounding, std::vector<Physical*> objects) {
	mBounds = bounding;

	mObjects = objects;

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;

	buildTree();
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

	int level = getLevel() % 5;
	if (level == 0)
		glColor3f(0x00, 0xFF, 0x00);
	if (level == 1)
		glColor3f(0xFF, 0x00, 0x00);
	if (level == 2)
		glColor3f(0x00, 0x00, 0xFF);
	if (level == 3)
		glColor3f(0x00, 0xFF, 0xFF);
	if (level == 4)
		glColor3f(0xFF, 0xFF, 0x00);

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
	//If there is only one object or no objects then don't subdivide
	if (mObjects.size() <= 1)
		return;

	//Get dimensions of the bounds
	Vec3 dim = mBounds.getMax() - mBounds.getMin();

	if (dim == Vec3::ZERO) {
		findNewCube();
		dim = mBounds.getMax() - mBounds.getMin();
	}

	//If the dimensions are smaller than the min size then stop
	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE && (int)dim.z <= MIN_SIZE)
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
	std::vector<Physical*> octList[8];

	//For objects that get moved down the tree
	std::vector<Physical*> delist;

	for (Physical* obj : mObjects) {
		if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax()) {
			for (int i = 0; i < 8; i++) {
				if (octants[i].intersect(obj->getBoundingBox())) {
					octList[i].push_back(obj);
					delist.push_back(obj);
					break;
				}
			}
		}
		else if (obj->getBoundingSphere().getRadius() != 0) {
			for (int i = 0; i < 8; i++) {
				if (octants[i].containsPoint(obj->getBoundingSphere().getCentre())) {
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
			mActiveNodes |= (unsigned char)(1 << i);
			mNodes[i]->buildTree();
		}
	}

	mBuilt = true;
	mReady = true;
}

Octree* Octree::createNode(AABB region, std::vector<Physical*> objects) {
	if (objects.size() == 0)
		return NULL;

	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

Octree* Octree::createNode(AABB region, Physical* object) {
	std::vector<Physical*> objects;
	objects.push_back(object);

	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

void Octree::add(std::vector<Physical*> objects) {
	for (Physical* obj : objects) {
		if (obj->mHasBounds) {
			Octree::mPendingInsertion.push_back(obj);
			mReady = true;
		}
	}
}

void Octree::add(Physical* object) {
	if (object->mHasBounds) {
		Octree::mPendingInsertion.push_back(object);
		mReady = true;
	}
}

void Octree::updateTree(unsigned int dt) {
	if (mBuilt) {
		if (mObjects.size() == 0) {
			if (!this->hasChildren()) {
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

		std::vector<Physical*> movedObjects;
		movedObjects.reserve(mObjects.size());

		for (Physical* obj : mObjects) {
			if (obj->hasMoved()) {
				movedObjects.push_back(obj);
			}
		}

		int size = mObjects.size();
		for (int i = 0; i < size; i++) {
			if (!mObjects[i]->isAlive()) {
				if (std::find(movedObjects.begin(), movedObjects.end(), mObjects[i]) != movedObjects.end()) {
					movedObjects.erase(std::remove(movedObjects.begin(), movedObjects.end(), mObjects[i]), movedObjects.end());
				}
				delete mObjects[i];
				mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), mObjects[i]), mObjects.end());
				size--;
			}
		}

		//Update active nodes
		int i1 = 0;
		for (unsigned char flags = mActiveNodes; flags > 0; flags >>= 1, i1++) {
			if ((flags & 1) == 1 && mNodes != NULL) mNodes[i1]->updateTree(dt);
		}

		//for (int i = 0; i < 8; i++) {
		//	bool flag = mActiveNodes >> i;
		//	if (mNodes[i] != NULL && mActiveNodes) {
		//		mNodes[i]->updateTree(dt);
		//	}
		//}

		//Moved objects code
		for (Physical* obj : movedObjects) {
			Octree* cur = this;

			if (obj->getBoundingBox(false).getMax() != obj->getBoundingBox(false).getMin()) {
				while (!cur->mBounds.intersect(obj->getBoundingBox())) {
					if (cur->mParent != NULL) {
						cur = cur->mParent;
					}
					else break;
				}
			}
			else {
				while (!cur->mBounds.containsPoint(obj->getBoundingSphere().getCentre())) {
					if (cur->mParent != NULL) {
						cur = cur->mParent;
					}
					else break;
				}
			}

			mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), obj), mObjects.end());
			cur->insertObject(obj);
		}

		movedObjects.clear();

		int i2 = 0;
		for (unsigned char flags = mActiveNodes; flags > 0; flags >>= 1, i2++) {
			if ((flags & 1) == 1 && mNodes[i2]->getCurrentLife() == 0) {
				delete mNodes[i2];
				mNodes[i2] = NULL;
				mActiveNodes ^= (unsigned char)(1 << i2);
			}
		}

		//if (mParent == NULL) {
		//	//Check Collisions
		//}
	}
	else {
		//buildTree();
	}
}

void Octree::updateTree() {
	if (!mBuilt) {
		for (int i = 0; i < mPendingInsertion.size(); i++) {
			mObjects.push_back(mPendingInsertion.at(i));
		}

		mPendingInsertion.clear();

		buildTree();
	}
	else {
		for (int i = 0; i < mPendingInsertion.size(); i++) {
			insertObject(mPendingInsertion.at(i));
		}

		mPendingInsertion.clear();
	}

	mReady = true;
}

void Octree::insertObject(Physical* obj) {
	if (mObjects.size() <= 1 && mActiveNodes == 0) {
		mObjects.push_back(obj);
		//this->add(obj);
		return;
	}

	Vec3 dim = mBounds.getMax() - mBounds.getMin();

	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE && (int)dim.z <= MIN_SIZE) {
		mObjects.push_back(obj);
		buildTree();
		return;
	}

	//if (getLevel() == MAX_LEVEL) {
	//	mObjects.push_back(obj);
	//	return;
	//}

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

	if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax() && mBounds.intersect(obj->getBoundingBox())) {
		bool found = false;

		for (int i = 0; i < 8; i++) {
			if (octants[i].intersect(obj->getBoundingBox())) {
				if (mNodes[i] != NULL) {
					mNodes[i]->insertObject(obj);
					found = true;
					break;
				}
				else {
					mNodes[i] = createNode(octants[i], obj);
					mActiveNodes |= (unsigned char)(1 << i);
					found = true;
					break;
				}
			}

		//	found = true;
		}

		if (!found) {
			mObjects.push_back(obj);
		}

		//buildTree();
	}
	else if (obj->getBoundingSphere().getRadius() != 0 && mBounds.containsPoint(obj->getBoundingSphere().getCentre())) {
		bool found = false;

		for (int i = 0; i < 8; i++) {
			if (octants[i].containsPoint(obj->getBoundingSphere().getCentre())) {
				if (mNodes[i] != NULL) {
					mNodes[i]->insertObject(obj);
					found = true;
					break;
				}
				else {
					mNodes[i] = createNode(octants[i], obj);
					mActiveNodes |= (unsigned char)(1 << i);
					found = true;
					break;
				}
			}
		}

		if (!found) {
			mObjects.push_back(obj);
		}
	} else {
		buildTree();
	}
}

int Octree::getCurrentLife() {
	return mCurLife;
}

Octree* Octree::getParent() {
	return mParent;
}

void Octree::findNewBoundary() {
	Vec3 globalMin = mBounds.getMin();
	Vec3 globalMax = mBounds.getMax();

	for (Physical* obj : mObjects) {
		Vec3 localMin = Vec3::ZERO;
		Vec3 localMax = Vec3::ZERO;

		if (!obj->mHasBounds) {
			return;
		}

		if (obj->getBoundingBox().getMax() != obj->getBoundingBox().getMin()) {
			localMin = obj->getBoundingBox().getMin();
			localMax = obj->getBoundingBox().getMax();
		}

		if (localMin.x < globalMin.x) globalMin.x = localMin.x;
		if (localMin.y < globalMin.y) globalMin.y = localMin.y;
		if (localMin.z < globalMin.z) globalMin.z = localMin.z;

		if (localMax.x > globalMax.x) globalMax.x = localMax.x;
		if (localMax.y > globalMax.y) globalMax.y = localMax.y;
		if (localMax.z > globalMax.z) globalMax.z = localMax.z;
	}

	mBounds.mMin = globalMin;
	mBounds.mMax = globalMax;
}

//int pow2roundup(int x)
//{
//	if (x < 0)
//		return 0;
//	--x;
//	x |= x >> 1;
//	x |= x >> 2;
//	x |= x >> 4;
//	x |= x >> 8;
//	x |= x >> 16;
//	return x + 1;
//}

void Octree::findNewCube() {
	findNewBoundary();

	Vec3 offset = mBounds.mMin - Vec3::ZERO;
	mBounds.mMin += offset;
	mBounds.mMax += offset;

	int highX = (int)std::floor(std::max(std::max(mBounds.mMax.x, mBounds.mMax.y), mBounds.mMax.z));

	for (int i = 0; i < 32; i++) {
		if (highX == 1 << i) {
			mBounds.mMax.x = highX;
			mBounds.mMax.y = highX;
			mBounds.mMax.z = highX;

			mBounds.mMin -= offset;
			mBounds.mMax -= offset;
			return;
		}
	}

	int x = std::pow(2, std::ceil(std::log(highX) / std::log(2)));
	mBounds.mMax.x = x;
	mBounds.mMax.y = x;
	mBounds.mMax.z = x;

	mBounds.mMin -= offset;
	mBounds.mMax -= offset;
}

Octree* Octree::getRoot() {
	Octree* root = mParent;

	while (root != nullptr)
		root = root->mParent;

	return root;
}

AABB Octree::getBounds() {
	if (this != nullptr)
		return mBounds;
	else
		return AABB(Vec3(0, 0, 0), Vec3(0, 0, 0));
}

bool Octree::hasChildren() {
	for (int i = 0; i < 8; i++) {
		if (mNodes[i] != NULL)
			return true;
	}

	return false;
}

std::vector<CollisionResult> Octree::getIntersection(AABB resgion) {
	std::vector<CollisionResult> results;

	if (mObjects.size() == 0 && hasChildren()) {
		return results;
	}

	for (Physical* obj : mObjects) {

	}
}

void Octree::printTree() {
	for (int i = 0; i < 8; i++) {
		if (this->mParent == NULL)
			printf("Root Node");

		switch (i) {
		case 0:
			printf("Front-bottom-left");
			break;
		case 1:
			printf("Front-bottom-right");
			break;
		case 2:
			printf("Back-bottom-right");
			break;
		case 3:
			printf("Back-bottom-left");
			break;
		case 4:
			printf("Front-top-left");
			break;
		case 5:
			printf("Front-top-right");
			break;
		case 6:
			printf("Back-top-right");
			break;
		case 7:
			printf("Back-top-left");
			break;
		}

		printf("\n");

		printf("Level: %i\n", this->getLevel());

		for (Physical* obj : this->mObjects) {
			printf("Object: x: %f, y: %f, z: %f, id: %i\n", obj->getPosition().x, obj->getPosition().y, obj->getPosition().z, obj->id);
		}

		printf("\n");

		if(mNodes[i] != NULL)
			mNodes[i]->printTree();
	}
}

int Octree::getLevel() {
	Octree* temp = this;
	int level = 0;

	while (temp->mParent != NULL) {
		temp = temp->mParent;
		level++;
	}

	return level;
}

std::vector<Physical*> Octree::checkQuery(AABB region) {
	std::vector<Physical*> objects;
	for (Physical* obj : mObjects) {
		if (obj->getBoundingBox(false).mMax != obj->getBoundingBox(false).mMin) {
			if (region.intersect(obj->getBoundingBox())) {
				objects.push_back(obj);
			}
			else
				obj->id = 0;
		}
	}

	for (int i = 0; i < 8; i++) {
		if (mNodes[i] != NULL) {
			std::vector<Physical*> nodeObjects = mNodes[i]->checkQuery(region);
			for (Physical* obj : nodeObjects) {
				objects.push_back(obj);
			}
		}
	}

	return objects;
}

int Octree::getObjectSize() {
	int size = mObjects.size();

	for (int i = 0; i < 8; i++) {
		if (mNodes[i] != NULL) {
			int newSize = mNodes[i]->getObjectSize();
			size += newSize;
		}
	}

	return size;
}