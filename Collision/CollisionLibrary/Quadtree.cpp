#include "Quadtree.h"
#include "GLShapes.h"
#include <algorithm>
#include <math.h>
#include <set>

//Checks the tree if ready to build or built
bool Quadtree::mReady = false;
bool Quadtree::mBuilt = false;
bool Quadtree::mNeedsResize = false;
bool Quadtree::mPrintChecks = false;
//Used to get new bounds
std::vector<Physical2D*> Quadtree::mOutboundObjects;
//Stores pending insertion objects (Not used)
std::vector<Physical2D*> Quadtree::mPendingInsertion;
//Test amount of collision checks for comparison to brute force
int Quadtree::mCollisionChecks = 0;

Quadtree::Quadtree(AABB2D bounding, std::vector<Physical2D*> objects) {
	//Set the bounds
	mBounds = bounding;

	//Set objects
	mObjects = objects;
	mOutboundObjects = objects;

	//Set lifespan for 8 cycles
	mMaxLifespan = 8;
	//Set current life
	mCurLife = -1;

	//Set active nodes (0 = no active nodes, 00000001 = Front-Bottom-Left active, etc...)
	mActiveNodes = 0;

	//Set parent to NULL which means it's root
	mParent = NULL;

	//Enable collision detection by default
	mBuiltInCheck = true;

	//Set all nodes to NULL
	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;

	//Build the tree with all objects
	buildTree();
}

Quadtree::Quadtree(AABB2D bounding) {
	//Set the bounds
	mBounds = bounding;

	//Set lifespan for 8 cycles
	mMaxLifespan = 8;
	//Set current life
	mCurLife = -1;

	//Set active nodes (0 = no active nodes, 00000001 = Front-Bottom-Left active, etc...)
	mActiveNodes = 0;

	//Set parent to NULL which means it's root
	mParent = NULL;

	//Enable collision detection by default
	mBuiltInCheck = true;

	//Set all nodes to NULL
	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;
}

Quadtree::Quadtree() {
	//Set bounds to zero (When objects are added and is built it will automatically get a bound)
	mBounds = AABB2D(Vec2::ZERO, Vec2::ZERO);

	//Set lifespan for 8 cycles
	mMaxLifespan = 8;
	//Set current life
	mCurLife = -1;
	mMaxLifespan = 8;
	mCurLife = -1;

	//Set active nodes (0 = no active nodes, 00000001 = Front-Bottom-Left active, etc...)
	mActiveNodes = 0;

	//Set parent to NULL which means it's root
	mParent = NULL;

	//Enable collision detection by default
	mBuiltInCheck = true;

	//Set all nodes to NULL
	for (int i = 0; i < 4; i++)
		mNodes[i] = NULL;
}

Quadtree::~Quadtree() {
	//Destroy all nodes
	for (int i = 0; i < 4; i++)
		delete mNodes[i];
}

void Quadtree::renderTree() {
	//Get all vertices for each corner of the bounds
	Vec2 verts[4];
	verts[0] = mBounds.mMin;
	verts[1] = Vec2(mBounds.mMin.x, mBounds.mMax.y);
	verts[2] = Vec2(mBounds.mMax.x, mBounds.mMin.y);
	verts[3] = mBounds.mMax;

	//Set line width of cube
	glLineWidth(1.0f);

	//Sets the colour based on the level of the tree (Repeats when gets deeper)
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

	//Draws the cube by drawing each line of the cube
	glBegin(GL_LINES);

	glVertex2f(verts[0].x, verts[0].y);
	glVertex2f(verts[1].x, verts[1].y);

	glVertex2f(verts[1].x, verts[1].y);
	glVertex2f(verts[3].x, verts[3].y);

	glVertex2f(verts[3].x, verts[3].y);
	glVertex2f(verts[2].x, verts[2].y);

	glVertex2f(verts[2].x, verts[2].y);
	glVertex2f(verts[0].x, verts[0].y);

	glEnd();

	//Recursivly render each node of the tree
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

	//If bounds are zero automatically find a bound
	if (dim == Vec2::ZERO) {
		findNewCube();
		dim = mBounds.mMax - mBounds.mMin;
	}

	//If the dimensions are smaller than the min size then stop
	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE)
		return;

	//Get the half size of the bound
	Vec2 halfDim = dim / 2.0f;
	//Get the centre of the cube
	Vec2 centre = mBounds.mMin + halfDim;

	//Store each octant bounds for checking
	AABB2D octants[4];
	//Bottom-Left
	octants[0] = AABB2D(mBounds.mMin, centre);
	//Bottom-Right
	octants[1] = AABB2D(Vec2(centre.x, mBounds.mMin.y), Vec2(mBounds.mMax.x, centre.y));
	//Top-Left
	octants[2] = AABB2D(Vec2(mBounds.mMin.x, centre.y), Vec2(centre.x, mBounds.mMax.y));
	//Top-Right
	octants[3] = AABB2D(centre, mBounds.mMax);

	//Stores objects for each octant
	std::vector<Physical2D*> octList[4];

	//For objects that get moved down the tree
	std::vector<Physical2D*> delist;

	//Go through each object and add if they are inside a octant and remove from parent
	for (Physical2D* obj : mObjects) {
		if (obj->getBoundingBox(false).mMin != obj->getBoundingBox(false).mMax) {
			for (int i = 0; i < 4; i++) {
				if (octants[i].intersect(obj->getBoundingBox())) {
					octList[i].push_back(obj);
					delist.push_back(obj);
					//break; //Removed because needed objects in multiple octants
				}
			}
		}
	}

	//Erase objects from parent (This object)
	for (int i = 0; i < delist.size(); i++) {
		mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), delist[i]), mObjects.end());
	}

	//Create the octants and set the active node flags then build the nodes recurivly building the tree
	for (int i = 0; i < 4; i++) {
		if (octList[i].size() != 0) {
			mNodes[i] = createNode(octants[i], octList[i]);
			mActiveNodes |= (unsigned char)(1 << i);
			mNodes[i]->buildTree();
		}
	}

	//Set flags for updating
	mBuilt = true;
	mReady = true;
}

Quadtree* Quadtree::createNode(AABB2D region, std::vector<Physical2D*> objects) {
	//Don't create the node if it has no objects
	if (objects.size() == 0)
		return NULL;

	//Create the node and set it's parent
	Quadtree* ret = new Quadtree(region, objects);
	ret->mParent = this;

	return ret;
}

Quadtree* Quadtree::createNode(AABB2D region, Physical2D* object) {
	//Add the single object to a vector as Quadtree nodes need a vector
	std::vector<Physical2D*> objects;
	objects.push_back(object);

	//Create the node and set it's parent
	Quadtree* ret = new Quadtree(region, objects);
	ret->mParent = this;

	return ret;
}

//Not used
void Quadtree::add(std::vector<Physical2D*> objects) {
	for (Physical2D* obj : objects) {
		if (obj->mHasBounds) {
			Quadtree::mPendingInsertion.push_back(obj);
			mReady = true;
		}
	}
}

//Not used
void Quadtree::add(Physical2D* object) {
	if (object->mHasBounds) {
		Quadtree::mPendingInsertion.push_back(object);
		mReady = true;
	}
}

void Quadtree::updateTree(unsigned int dt) {
	//If the tree is built then we update
	if (mBuilt) {
		//Reset collision check counter
		mCollisionChecks = 0;

		//Set timer for this node once node runs out it will be removed
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

		//Stores objects which have moved
		std::vector<Physical2D*> movedObjects;
		movedObjects.reserve(mObjects.size());

		//Check if objects have moved
		for (Physical2D* obj : mObjects) {
			if (obj->hasMoved()) {
				movedObjects.push_back(obj);
			}
		}

		//Get size objects
		int size = mObjects.size();

		//Checks if objects are still alive and if not then delete them and remove from the tree;
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

		//Moved objects code
		for (Physical2D* obj : movedObjects) {
			//Set current tree node
			Quadtree* cur = this;

			//Check collision with other nodes
			if (obj->getBoundingBox(false).mMax != obj->getBoundingBox(false).mMin) {
				while (!cur->mBounds.intersect(obj->getBoundingBox())) {
					if (cur->mParent != NULL) {
						cur = cur->mParent;
					}
					else break;
				}
			}

			//Remove object from this node and insert in new node
			mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), obj), mObjects.end());
			cur->insertObject(obj);

			//Check if still in bounds else rebuild the tree with new size
			Quadtree* root;
			if (this->mParent == NULL)
				root = this;
			else
				root = this->getRoot();

			if (!obj->getBoundingBox().intersect(root->mBounds)) {
				//Add the object to the root of the tree
				root->mObjects.push_back(obj);
				mOutboundObjects.push_back(obj);

				//Needs to resize the tree
				mNeedsResize = true;
			}
		}

		//Clear all moved objects
		movedObjects.clear();

		//Delete inactive children nodes
		int i2 = 0;
		for (unsigned char flags = mActiveNodes; flags > 0; flags >>= 1, i2++) {
			if ((flags & 1) == 1 && mNodes[i2]->getCurrentLife() == 0) {
				delete mNodes[i2];
				mNodes[i2] = NULL;
				mActiveNodes ^= (unsigned char)(1 << i2);
			}
		}

		//Do collision checks
		if (mParent == NULL) {
			if (mBuiltInCheck) {
				std::vector<CollisionResult2D*> cr = getIntersection(mObjects);

				if (mPrintChecks) {
					printf("Collision Checks: %i\n\n", mCollisionChecks);
					mPrintChecks = false;
				}

				for (CollisionResult2D* c : cr) {
					if (c->mCollisionObject != NULL) {
						c->mCollisionObject->handleCollision(c->mOther);
					}
					if (c->mOther != NULL) {
						c->mOther->handleCollision(c->mCollisionObject);
					}
				}
			}

			if (mNeedsResize) {
				findNewBoundary();

				this->resize(mBounds);

				mNeedsResize = false;

				mOutboundObjects.clear();
			}
		}
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

	Quadtree* root;
	if (this->mParent == NULL)
		root = this;
	else
		root = this->getRoot();

	if (!obj->getBoundingBox().intersect(root->mBounds)) {
		root->mObjects.push_back(obj);
		mOutboundObjects.push_back(obj);

		mNeedsResize = true;

		return;
	}

	Vec2 halfDim = dim / 2.0f;
	Vec2 centre = mBounds.mMin + halfDim;

	AABB2D octants[4];
	//Bottom-Left
	octants[0] = (mNodes[0] != NULL) ? mNodes[0]->mBounds : AABB2D(mBounds.mMin, centre);
	//Bottom-Right
	octants[1] = (mNodes[1] != NULL) ? mNodes[1]->mBounds : AABB2D(Vec2(centre.x, mBounds.mMin.y), Vec2(mBounds.mMax.x, centre.y));
	//Top-Left
	octants[2] = (mNodes[2] != NULL) ? mNodes[2]->mBounds : AABB2D(Vec2(mBounds.mMin.x, centre.y), Vec2(centre.x, mBounds.mMax.y));
	//Top-Right
	octants[3] = (mNodes[3] != NULL) ? mNodes[3]->mBounds : AABB2D(centre, mBounds.mMax);

	if (obj->getBoundingBox(false).mMin != obj->getBoundingBox(false).mMax && mBounds.intersect(obj->getBoundingBox())) {
		bool found = false;

		for (int i = 0; i < 4; i++) {
			if (octants[i].intersect(obj->getBoundingBox())) {
				found = true;
				if (mNodes[i] != NULL) {
					mNodes[i]->insertObject(obj);
					//break;
				}
				else {
					mNodes[i] = createNode(octants[i], obj);
					mActiveNodes |= (unsigned char)(1 << i);
					//break;
				}
			}
		}

		if (!found) {
			mObjects.push_back(obj);
		}
	}
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

	for (Physical2D* obj : mOutboundObjects) {
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

	while (root->mParent != NULL) {
		root = root->mParent;
	}

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

//Broken
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

	if (region.intersect(mBounds)) {
		for (Physical2D* obj : mObjects) {
			objects.push_back(obj);
		}
	}

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			if (mNodes[i]->getBounds().intersect(region)) {
				std::vector<Physical2D*> nodeObjects = mNodes[i]->checkQuery(region);
				for (Physical2D* obj : nodeObjects) {
					objects.push_back(obj);
				}
			}
		}
	}

	return objects;
}

int Quadtree::getObjectSize(bool duplicate) {
	int size = 0;

	if (duplicate) {
		std::vector<Physical2D*> objects = getAllObjects();

		size = objects.size();
	}
	else {
		std::vector<Physical2D*> objects = getAllObjects();
		std::set<Physical2D*> setObjects(objects.begin(), objects.end());

		size = setObjects.size();
	}

	return size;
}

bool isStatic(Physical2D* o) {
	return o->mStatic;
}

std::vector<CollisionResult2D*> Quadtree::getIntersection(std::vector<Physical2D*> objects) {
	std::vector<CollisionResult2D*> intersections;
	std::set<Physical2D*> reduce(objects.begin(), objects.end());

	for (Physical2D* obj1 : reduce) {
		for (Physical2D* obj2 : mObjects) {
			if (obj1 == obj2 || (obj1->mStatic && obj2->mStatic))
				continue;

			if (obj1->getBoundingBox().intersect(obj2->getBoundingBox())) {
				CollisionResult2D* newCollision = new CollisionResult2D(obj1, obj2);
				intersections.push_back(newCollision);
			}
			mCollisionChecks++;
		}
	}

	if (mObjects.size() > 1) {
		std::vector<Physical2D*> temp = mObjects;
		while (temp.size() > 0) {
			for (Physical2D* obj : temp) {
				if (temp[temp.size() - 1] == obj || (temp[temp.size() - 1]->mStatic && obj->mStatic)) {
					continue;
				}
				if (temp[temp.size() - 1]->getBoundingBox().intersect(obj->getBoundingBox())) {
					CollisionResult2D* newCollision = new CollisionResult2D(temp[temp.size() - 1], obj);
					intersections.push_back(newCollision);
				}
				mCollisionChecks++;
			}

			temp.erase(std::find(temp.begin(), temp.end(), temp.at(temp.size() - 1)), temp.end());
		}
	}

	for (Physical2D* obj : mObjects) {
		objects.push_back(obj);
	}

	int i = 0;
	for (int flags = mActiveNodes; flags > 0; i++, flags >>= 1) {
		if ((flags & 1) == 1) {
			std::vector<CollisionResult2D*> newIntersections = mNodes[i]->getIntersection(objects);
			intersections.insert(std::end(intersections), std::begin(newIntersections), std::end(newIntersections));
		}
	}

	return intersections;
}

std::vector<Physical2D*> Quadtree::getAllObjects() {
	std::vector<Physical2D*> objects;

	for (Physical2D* obj : mObjects) {
		objects.push_back(obj);
	}

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			std::vector<Physical2D*> result = mNodes[i]->getAllObjects();

			for (Physical2D* obj : result) {
				objects.push_back(obj);
			}
		}
	}

	return objects;
}

void Quadtree::resize(AABB2D region) {
	if (mParent != NULL) {
		return;
	}

	std::vector<Physical2D*> rawObjects = this->getAllObjects();
	std::set<Physical2D*> setObjects(rawObjects.begin(), rawObjects.end());

	for (int i = 0; i < 4; i++) {
		if (mNodes[i] != NULL) {
			delete mNodes[i];
			mNodes[i] = NULL;
		}
	}

	//Reset variables
	mActiveNodes = 0;
	mCollisionChecks = 0;
	mMaxLifespan = 8;
	mCurLife = -1;

	mObjects.clear();
	mBounds = region;

	for (Physical2D* obj : setObjects) {
		this->insertObject(obj);
	}
}

void Quadtree::printDetails() {
	printf("Objects in tree: %i\n", getObjectSize(false));
	printf("Objects in tree (Including Duplicates): %i\n", getObjectSize());
	mPrintChecks = true;
}

bool Quadtree::isCollisionEnabled() {
	return mBuiltInCheck;
}

void Quadtree::toggleCollision() {
	mBuiltInCheck = !mBuiltInCheck;
}