#include "Octree.h"
#include "GLShapes.h"
#include <algorithm>
#include <math.h>
#include <set>

//Checks the tree if ready to build or built
bool Octree::mReady = false;
bool Octree::mBuilt = false;
bool Octree::mNeedsResize = false;
bool Octree::mPrintChecks = false;
//Used to get new bounds
std::vector<Physical*> mOutboundObjects;
//Stores pending insertion objects (Not used)
std::vector<Physical*> Octree::mPendingInsertion;
//Test amount of collision checks for comparison to brute force
int Octree::mCollisionChecks = 0;

Octree::Octree(AABB bounding, std::vector<Physical*> objects) {
	//Set the bounds
	mBounds = bounding;

	//Set objects
	mObjects = objects;

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
	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;

	//Build the tree with all objects
	buildTree();
}

Octree::Octree(AABB bounding) {
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
	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;
}

Octree::Octree() {
	//Set bounds to zero (When objects are added and is built it will automatically get a bound)
	mBounds = AABB(Vec3::ZERO, Vec3::ZERO);

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
	for (int i = 0; i < 8; i++)
		mNodes[i] = NULL;
}

Octree::~Octree() {
	//Destroy all nodes
	for (int i = 0; i < 8; i++)
		delete mNodes[i];
}

void Octree::renderTree() {
	//Get all vertices for each corner of the bounds
	Vec3 verts[8];
	verts[0] = mBounds.getMin();
	verts[1] = Vec3(mBounds.getMin().x, mBounds.getMin().y, mBounds.getMax().z);
	verts[2] = Vec3(mBounds.getMin().x, mBounds.getMax().y, mBounds.getMin().z);
	verts[3] = Vec3(mBounds.getMax().x, mBounds.getMin().y, mBounds.getMin().z);

	verts[7] = mBounds.getMax();
	verts[4] = Vec3(mBounds.getMax().x, mBounds.getMax().y, mBounds.getMin().z);
	verts[5] = Vec3(mBounds.getMax().x, mBounds.getMin().y, mBounds.getMax().z);
	verts[6] = Vec3(mBounds.getMin().x, mBounds.getMax().y, mBounds.getMax().z);

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

	//Recursivly render each node of the tree
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

	//If bounds are zero automatically find a bound
	if (dim == Vec3::ZERO) {
		findNewCube();
		dim = mBounds.getMax() - mBounds.getMin();
	}

	//If the dimensions are smaller than the min size then stop
	if ((int)dim.x <= MIN_SIZE && (int)dim.y <= MIN_SIZE && (int)dim.z <= MIN_SIZE)
		return;

	//Get the half size of the bound
	Vec3 halfDim = dim / 2.0f;
	//Get the centre of the cube
	Vec3 centre = mBounds.getMin() + halfDim;

	//Store each octant bounds for checking
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

	//Go through each object and add if they are inside a octant and remove from parent
	for (Physical* obj : mObjects) {
		if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax()) {
			for (int i = 0; i < 8; i++) {
				if (octants[i].intersect(obj->getBoundingBox())) {
					octList[i].push_back(obj);
					delist.push_back(obj);
					//break; //Removed because needed objects in multiple octants
				}
			}
		}

		//Same as above but with sphere
		else if (obj->getBoundingSphere().getRadius() != 0) {
			for (int i = 0; i < 8; i++) {
				if (octants[i].containsPoint(obj->getBoundingSphere().getCentre())) {
					octList[i].push_back(obj);
					delist.push_back(obj);
					//break;
				}
			}
		}
	}

	//Erase objects from parent (This object)
	for (int i = 0; i < delist.size(); i++) {
		mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), delist[i]), mObjects.end());
	}

	//Create the octants and set the active node flags then build the nodes recurivly building the tree
	for (int i = 0; i < 8; i++) {
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

Octree* Octree::createNode(AABB region, std::vector<Physical*> objects) {
	//Don't create the node if it has no objects
	if (objects.size() == 0)
		return NULL;

	//Create the node and set it's parent
	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

Octree* Octree::createNode(AABB region, Physical* object) {
	//Add the single object to a vector as Octree nodes need a vector
	std::vector<Physical*> objects;
	objects.push_back(object);

	//Create the node and set it's parent
	Octree* ret = new Octree(region, objects);
	ret->mParent = this;

	return ret;
}

//Not used
void Octree::add(std::vector<Physical*> objects) {
	for (Physical* obj : objects) {
		if (obj->mHasBounds) {
			Octree::mPendingInsertion.push_back(obj);
			mReady = true;
		}
	}
}

//Not used
void Octree::add(Physical* object) {
	if (object->mHasBounds) {
		Octree::mPendingInsertion.push_back(object);
		mReady = true;
	}
}

void Octree::updateTree(unsigned int dt) {
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
		std::vector<Physical*> movedObjects;
		movedObjects.reserve(mObjects.size());

		//Check if objects have moved
		for (Physical* obj : mObjects) {
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
		for (Physical* obj : movedObjects) {
			//Set current tree node
			Octree* cur = this;

			//Check collision with other nodes
			if (obj->getBoundingBox(false).getMax() != obj->getBoundingBox(false).getMin()) {
				while (!cur->mBounds.intersect(obj->getBoundingBox())) {
					if (cur->mParent != NULL) {
						cur = cur->mParent;
					}
					else break;
				}
			}
			else {
				//Same as above but with sphere
				while (!cur->mBounds.containsPoint(obj->getBoundingSphere().getCentre())) {
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
			Octree* root;
			if (this->mParent == NULL)
				root = this;
			else
				root = this->getRoot();

			if (!obj->getBoundingBox().intersect(root->mBounds)) {
				//Add the object to the root of the tree
				root->mObjects.push_back(obj);

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
				std::vector<CollisionResult*> cr = getIntersection(mObjects);

				if (mPrintChecks) {
					printf("Collision Checks: %i\n\n", mCollisionChecks);
					mPrintChecks = false;
				}

				for (CollisionResult* c : cr) {
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
			}
		}
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

	Octree* root;
	if (this->mParent == NULL)
		root = this;
	else
		root = this->getRoot();

	if (!obj->getBoundingBox().intersect(root->mBounds)) {
		root->mObjects.push_back(obj);

		mNeedsResize = true;

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

	if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax() && mBounds.intersect(obj->getBoundingBox())) {
		bool found = false;

		for (int i = 0; i < 8; i++) {
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

	while (root->mParent != NULL) {
		root = root->mParent;
	}

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

//Broken
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
	//for (Physical* obj : mObjects) {
	//	if (obj->getBoundingBox(false).mMax != obj->getBoundingBox(false).mMin) {
	//		if (region.intersect(obj->getBoundingBox())) {
	//			objects.push_back(obj);
	//		}
	//	}
	//}

	if (region.intersect(mBounds)) {
		for (Physical* obj : mObjects) {
			objects.push_back(obj);
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

int Octree::getObjectSize(bool duplicate) {
	int size = 0;

	if (duplicate) {
		std::vector<Physical*> objects = getAllObjects();

		size = objects.size();
	}
	else {
		std::vector<Physical*> objects = getAllObjects();
		std::set<Physical*> setObjects(objects.begin(), objects.end());

		size = setObjects.size();
	}

	return size;
}

std::vector<CollisionResult*> Octree::getIntersection(std::vector<Physical*> objects) {
	std::vector<CollisionResult*> intersections;
	std::set<Physical*> reduce(objects.begin(), objects.end());

	for (Physical* obj1 : reduce) {
		for (Physical* obj2 : mObjects) {
			if (obj1 == obj2 || (obj1->mStatic && obj2->mStatic))
				continue;

			if (obj1->getBoundingBox().intersect(obj2->getBoundingBox())) {
				CollisionResult* newCollision = new CollisionResult(obj1, obj2);
				intersections.push_back(newCollision);
			}
			mCollisionChecks++;
		}
	}

	if (mObjects.size() > 1) {
		std::vector<Physical*> temp = mObjects;
		while (temp.size() > 0) {
			for (Physical* obj : temp) {
				if (temp[temp.size() - 1] == obj || (temp[temp.size() - 1]->mStatic && obj->mStatic)) {
					continue;
				}
				if (temp[temp.size() - 1]->getBoundingBox().intersect(obj->getBoundingBox())) {
					CollisionResult* newCollision = new CollisionResult(temp[temp.size() - 1], obj);
					intersections.push_back(newCollision);
				}
				mCollisionChecks++;
			}

			temp.erase(std::find(temp.begin(), temp.end(), temp.at(temp.size() - 1)), temp.end());
		}
	}

	for (Physical* obj : mObjects) {
		//if(!obj->mStatic)
			objects.push_back(obj);
	}

	int i = 0;
	for (int flags = mActiveNodes; flags > 0; i++, flags >>= 1) {
		if ((flags & 1) == 1) {
			std::vector<CollisionResult*> newIntersections = mNodes[i]->getIntersection(objects);
			intersections.insert(std::end(intersections), std::begin(newIntersections), std::end(newIntersections));
		}
	}

	return intersections;
}

std::vector<Physical*> Octree::getAllObjects() {
	std::vector<Physical*> objects;

	for (Physical* obj : mObjects) {
		objects.push_back(obj);
	}

	for (int i = 0; i < 8; i++) {
		if (mNodes[i] != NULL) {
			std::vector<Physical*> result = mNodes[i]->getAllObjects();

			for (Physical* obj : result) {
				objects.push_back(obj);
			}
		}
	}

	return objects;
}

void Octree::resize(AABB region) {
	if (mParent != NULL) {
		return;
	}

	std::vector<Physical*> rawObjects = this->getAllObjects();
	std::set<Physical*> setObjects(rawObjects.begin(), rawObjects.end());

	for (int i = 0; i < 8; i++) {
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

	for (Physical* obj : setObjects) {
		this->insertObject(obj);
	}
}

void Octree::printDetails() {
	printf("Objects in tree: %i\n", getObjectSize(false));
	printf("Objects in tree (Including Duplicates): %i\n", getObjectSize());
	mPrintChecks = true;
}

bool Octree::isCollisionEnabled() {
	return mBuiltInCheck;
}

void Octree::toggleCollision() {
	mBuiltInCheck = !mBuiltInCheck;
}