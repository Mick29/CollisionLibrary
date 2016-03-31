#include "Quadtree.h"
#include "GLShapes.h"
#include <algorithm>
#include <math.h>
#include "LUtil.h"

bool Quadtree::mReady = false;
bool Quadtree::mBuilt = false;
std::vector<Physical2D*> Quadtree::mPendingInsertion;

Quadtree::Quadtree(AABB2D bounding, std::vector<Physical2D*> objects) {
	mBounds = bounding;

	mObjects = objects;

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;

	buildTree();
}

Quadtree::Quadtree(AABB2D bounding) {
	mBounds = bounding;

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;
}

Quadtree::Quadtree() {
	mBounds = AABB2D(Vec2::ZERO, Vec2::ZERO);

	mMaxLifespan = 8;
	mCurLife = -1;

	mActiveNodes = 0;

	mParent = NULL;

	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;
}

Quadtree::~Quadtree() {
	for (int i = 0; i < 4; i++)
		delete mNodes[i];
}

void Quadtree::renderTree() {
	//glLineWidth(1.0f);
	//glTranslatef(gCameraX, gCameraY, 0.0f);

	glBegin(GL_LINES);
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

		glVertex2f(mBounds.mMin.x, mBounds.mMin.y);
		glVertex2f(mBounds.mMin.x, mBounds.mMax.y);

		glVertex2f(mBounds.mMin.x, mBounds.mMax.y);
		glVertex2f(mBounds.mMax.x, mBounds.mMax.y);

		glVertex2f(mBounds.mMax.x, mBounds.mMax.y);
		glVertex2f(mBounds.mMax.x, mBounds.mMin.y);

		glVertex2f(mBounds.mMax.x, mBounds.mMin.y);
		glVertex2f(mBounds.mMin.x, mBounds.mMin.y);
	glEnd();

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			mNodes[i]->renderTree();
		}
	}
}

void Quadtree::buildTree() {
	//If there is only one object or no objects then don't subdivide
	if (mObjects.size() <= 1)
		return;

	//Get dimensions of the bounds
	Vec2 dim = mBounds.mMax - mBounds.mMin;

	if (dim == Vec2::ZERO) {
		findNewCube();
		dim = mBounds.mMax - mBounds.mMin;
	}

	//If the dimensions are smaller than the min size then stop
	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE)
		return;

	Vec2 halfDim = dim / 2.0f;
	Vec2 centre = mBounds.mMin + halfDim;

	AABB2D quads[4];
	//Bottom-Left
	quads[0] = AABB2D(mBounds.mMin, centre);
	//Bottom-Right
	quads[1] = AABB2D(Vec2(centre.x, mBounds.mMin.y), Vec2(mBounds.mMax.x, centre.y));
	//Top-Left
	quads[2] = AABB2D(Vec2(mBounds.mMin.x, centre.y), Vec2(centre.x, mBounds.mMax.y));
	//Top-Right
	quads[3] = AABB2D(centre, mBounds.mMax);

	//Stores objects for each octant
	std::vector<Physical2D*> quadList[4];

	//For objects that get moved down the tree
	std::vector<Physical2D*> delist;

	for (Physical2D* obj : mObjects) {
		if (obj->getBoundingBox(false).mMin != obj->getBoundingBox(false).mMax) {
			for (int i = 0; i < 4; i++) {
				if (quads[i].intersect(obj->getBoundingBox())) {
					quadList[i].push_back(obj);
					delist.push_back(obj);
					break;
				}
			}
		}
		//else if (obj->getBoundingSphere().getRadius() != 0) {
		//	for (int i = 0; i < 8; i++) {
		//		if (octants[i].containsPoint(obj->getBoundingSphere().getCentre())) {
		//			octList[i].push_back(obj);
		//			delist.push_back(obj);
		//			break;
		//		}
		//	}
		//}
	}

	for (int i = 0; i < delist.size(); i++) {
		mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), delist[i]), mObjects.end());
	}

	for (int i = 0; i < 4; i++) {
		if (quadList[i].size() != 0) {
			mNodes[i] = createNode(quads[i], quadList[i]);
			mActiveNodes |= (unsigned char)(1 << i);
			mNodes[i]->buildTree();
		}
	}

	mBuilt = true;
	mReady = true;
}

Quadtree* Quadtree::createNode(AABB2D region, std::vector<Physical2D*> objects) {
	if (objects.size() == 0)
		return NULL;

	Quadtree* ret = new Quadtree(region, objects);
	ret->mParent = this;

	return ret;
}

Quadtree* Quadtree::createNode(AABB2D region, Physical2D* object) {
	std::vector<Physical2D*> objects;
	objects.push_back(object);

	Quadtree* ret = new Quadtree(region, objects);
	ret->mParent = this;

	return ret;
}

void Quadtree::add(std::vector<Physical2D*> objects) {
	for (Physical2D* obj : objects) {
		if (obj->mHasBounds) {
			Quadtree::mPendingInsertion.push_back(obj);
			mReady = true;
		}
	}
}

void Quadtree::add(Physical2D* object) {
	if (object->mHasBounds) {
		Quadtree::mPendingInsertion.push_back(object);
		mReady = true;
	}
}

void Quadtree::updateTree(unsigned int dt) {
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

		std::vector<Physical2D*> movedObjects;
		movedObjects.reserve(mObjects.size());

		for (Physical2D* obj : mObjects) {
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
		for (Physical2D* obj : movedObjects) {
			Quadtree* cur = this;

			if (obj->getBoundingBox(false).mMax != obj->getBoundingBox(false).mMin) {
				while (!cur->mBounds.intersect(obj->getBoundingBox())) {
					if (cur->mParent != NULL) {
						cur = cur->mParent;
					}
					else break;
				}
			}
			//else {
			//	while (!cur->mBounds.containsPoint(obj->getBoundingSphere().getCentre())) {
			//		if (cur->mParent != NULL) {
			//			cur = cur->mParent;
			//		}
			//		else break;
			//	}
			//}

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
		if (mParent == NULL) {

		}
	}
	else {
		//buildTree();
	}
}

void Quadtree::updateTree() {
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

void Quadtree::insertObject(Physical2D* obj) {
	if (mObjects.size() <= 1 && mActiveNodes == 0) {
		mObjects.push_back(obj);
		//this->add(obj);
		return;
	}

	Vec2 dim = mBounds.mMax - mBounds.mMin;

	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE) {
		mObjects.push_back(obj);
		buildTree();
		return;
	}

	//if (getLevel() == MAX_LEVEL) {
	//	mObjects.push_back(obj);
	//	return;
	//}

	Vec2 halfDim = dim / 2.0f;
	Vec2 centre = mBounds.mMin + halfDim;

	AABB2D quads[4];
	//Bottom-Left
	quads[0] = (mNodes[0] != NULL) ? mNodes[0]->mBounds : AABB2D(mBounds.mMin, centre);
	//Bottom-Right
	quads[1] = (mNodes[1] != NULL) ? mNodes[1]->mBounds : AABB2D(Vec2(centre.x, mBounds.mMin.y), Vec2(mBounds.mMax.x, centre.y));
	//Top-Left
	quads[2] = (mNodes[2] != NULL) ? mNodes[2]->mBounds : AABB2D(Vec2(mBounds.mMin.x, centre.y), Vec2(centre.x, mBounds.mMax.y));
	//Top-Right
	quads[3] = (mNodes[3] != NULL) ? mNodes[3]->mBounds : AABB2D(centre, mBounds.mMax);

	if (obj->getBoundingBox(false).mMin != obj->getBoundingBox(false).mMax && mBounds.intersect(obj->getBoundingBox())) {
		bool found = false;

		for (int i = 0; i < 4; i++) {
			if (quads[i].intersect(obj->getBoundingBox())) {
				if (mNodes[i] != NULL) {
					mNodes[i]->insertObject(obj);
					found = true;
					break;
				}
				else {
					mNodes[i] = createNode(quads[i], obj);
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
	//else if (obj->getBoundingSphere().getRadius() != 0 && mBounds.containsPoint(obj->getBoundingSphere().getCentre())) {
	//	bool found = false;

	//	for (int i = 0; i < 8; i++) {
	//		if (octants[i].containsPoint(obj->getBoundingSphere().getCentre())) {
	//			if (mNodes[i] != NULL) {
	//				mNodes[i]->insertObject(obj);
	//				found = true;
	//				break;
	//			}
	//			else {
	//				mNodes[i] = createNode(octants[i], obj);
	//				mActiveNodes |= (unsigned char)(1 << i);
	//				found = true;
	//				break;
	//			}
	//		}
	//	}

	//	if (!found) {
	//		mObjects.push_back(obj);
	//	}
	//}
	else {
		buildTree();
	}
}

int Quadtree::getCurrentLife() {
	return mCurLife;
}

Quadtree* Quadtree::getParent() {
	return mParent;
}

void Quadtree::findNewBoundary() {
	Vec2 globalMin = mBounds.mMin;
	Vec2 globalMax = mBounds.mMax;

	for (Physical2D* obj : mObjects) {
		Vec2 localMin = Vec2::ZERO;
		Vec2 localMax = Vec2::ZERO;

		if (!obj->mHasBounds) {
			return;
		}

		if (obj->getBoundingBox().mMax != obj->getBoundingBox().mMin) {
			localMin = obj->getBoundingBox().mMin;
			localMax = obj->getBoundingBox().mMax;
		}

		if (localMin.x < globalMin.x) globalMin.x = localMin.x;
		if (localMin.y < globalMin.y) globalMin.y = localMin.y;

		if (localMax.x > globalMax.x) globalMax.x = localMax.x;
		if (localMax.y > globalMax.y) globalMax.y = localMax.y;
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

void Quadtree::findNewCube() {
	findNewBoundary();

	Vec2 offset = mBounds.mMin - Vec2::ZERO;
	mBounds.mMin += offset;
	mBounds.mMax += offset;

	int highX = (int)std::floor(std::max(mBounds.mMax.x, mBounds.mMax.y));

	for (int i = 0; i < 32; i++) {
		if (highX == 1 << i) {
			mBounds.mMax.x = highX;
			mBounds.mMax.y = highX;

			mBounds.mMin -= offset;
			mBounds.mMax -= offset;
			return;
		}
	}

	int x = std::pow(2, std::ceil(std::log(highX) / std::log(2)));
	mBounds.mMax.x = x;
	mBounds.mMax.y = x;

	mBounds.mMin -= offset;
	mBounds.mMax -= offset;
}

Quadtree* Quadtree::getRoot() {
	Quadtree* root = mParent;

	while (root != nullptr)
		root = root->mParent;

	return root;
}

AABB2D Quadtree::getBounds() {
	if (this != nullptr)
		return mBounds;
	else
		return AABB2D(Vec2(0, 0), Vec2(0, 0));
}

bool Quadtree::hasChildren() {
	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL)
			return true;
	}

	return false;
}

std::vector<CollisionResult> Quadtree::getIntersection(AABB2D region) {
	std::vector<CollisionResult> results;

	if (mObjects.size() == 0 && hasChildren()) {
		return results;
	}

	for (Physical2D* obj : mObjects) {

	}
}

void Quadtree::printTree() {
	for (int i = 0; i < 4; i++) {
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

		for (Physical2D* obj : this->mObjects) {
			printf("Object: x: %f, y: %f, id: %i\n", obj->getPosition().x, obj->getPosition().y, obj->id);
		}

		printf("\n");

		if (mNodes[i] != NULL)
			mNodes[i]->printTree();
	}
}

int Quadtree::getLevel() {
	Quadtree* temp = this;
	int level = 0;

	while (temp->mParent != NULL) {
		temp = temp->mParent;
		level++;
	}

	return level;
}

std::vector<Physical2D*> Quadtree::checkQuery(AABB2D region) {
	std::vector<Physical2D*> objects;
	for (Physical2D* obj : mObjects) {
		if (obj->getBoundingBox(false).mMax != obj->getBoundingBox(false).mMin) {
			if (region.intersect(obj->getBoundingBox())) {
				objects.push_back(obj);
			}
			else
				obj->id = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			std::vector<Physical2D*> nodeObjects = mNodes[i]->checkQuery(region);
			for (Physical2D* obj : nodeObjects) {
				objects.push_back(obj);
			}
		}
	}

	return objects;
}

int Quadtree::getObjectSize() {
	int size = mObjects.size();

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			int newSize = mNodes[i]->getObjectSize();
			size += newSize;
		}
	}

	return size;
}