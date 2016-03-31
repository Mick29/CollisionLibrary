#pragma once

#include "Vector.h"
#include "AABB.h"
#include "Physical.h"
#include <vector>
#include <queue>
#include <bitset>
#include "CollisionResult.h"

class Quadtree {
public:
	Quadtree(AABB2D bounding, std::vector<Physical2D*> objects);
	Quadtree(AABB2D bounding);
	Quadtree();
	~Quadtree();

	void buildTree();
	void renderTree();
	void updateTree(unsigned int dt);
	void updateTree();
	void insertObject(Physical2D* obj);

	Quadtree* createNode(AABB2D region, std::vector<Physical2D*> objects);
	Quadtree* createNode(AABB2D region, Physical2D* object);

	void add(std::vector<Physical2D*> objects);
	void add(Physical2D* object);

	Quadtree* getRoot();

	AABB2D getBounds();

	bool hasChildren();

	std::vector<CollisionResult2D*> getIntersection(std::vector<Physical2D*> objects);

	void printTree();
	int getLevel();

	//Get amount of objects (duplicate == true then duplicate objects are counted)
	int getObjectSize(bool duplicate = true);

	std::vector<Physical2D*> checkQuery(AABB2D region);

	//Resizes the tree
	void resize(AABB2D region);

	//Gets all the objects in the tree
	std::vector<Physical2D*> getAllObjects();

	//Prints details on the tree
	void printDetails();
private:
	int getCurrentLife();

	Quadtree* getParent();

	void findNewBoundary();
	void findNewCube();
	//Defines bounds for region
	AABB2D mBounds;
	//Children of the octree
	Quadtree* mNodes[4];
	//Parent reference
	Quadtree* mParent;
	//Store objects
	std::vector<Physical2D*> mObjects;
	//Minimum size of bounding box (1*1*1)
	const int MIN_SIZE = 1;
	const int MAX_LEVEL = 5;
	//Objects waiting to be inserted
	static std::vector<Physical2D*> mPendingInsertion;
	//Used to check which nodes are active (eg 00000001 -> the first node (top-front-left) is active)
	unsigned char mActiveNodes;
	//Frames to wait until we insert
	int mMaxLifespan;
	int mCurLife;
	//Bools to tell if tree is ready and built
	static bool mReady;	//If false there is insertions left and needs to be built again (Updates)
	static bool mBuilt;	//If false the tree was not built yet
	static bool mNeedsResize;  //If there is objects outside of tree then this is set
	static bool mPrintChecks;  //If we want to print collision checks then this is set
	static int mCollisionChecks;  //Counts number of collision checks for comparing to brute force
	static int mObjectCheck;
};