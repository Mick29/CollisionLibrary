#pragma once

#include "Vector.h"
#include "AABB.h"
#include "Physical.h"
#include <vector>
#include <queue>
#include <bitset>
#include "CollisionResult.h"

class Octree {
public:
	Octree(AABB bounding, std::vector<Physical*> objects);
	Octree(AABB bounding);
	Octree();
	~Octree();

	void buildTree();
	void renderTree();
	void updateTree(unsigned int dt);
	void updateTree();
	void insertObject(Physical* obj);

	Octree* createNode(AABB region, std::vector<Physical*> objects);
	Octree* createNode(AABB region, Physical* object);

	void add(std::vector<Physical*> objects);
	void add(Physical* object);

	Octree* getRoot();

	AABB getBounds();

	bool hasChildren();

	std::vector<CollisionResult> getIntersection(AABB resgion);

	void printTree();
	int getLevel();
	int getObjectSize();

	std::vector<Physical*> checkQuery(AABB region);
private:
	int getCurrentLife();

	Octree* getParent();

	void findNewBoundary();
	void findNewCube();
	//Defines bounds for region
	AABB mBounds;
	//Children of the octree
	Octree* mNodes[8];
	//Parent reference
	Octree* mParent;
	//Store objects
	std::vector<Physical*> mObjects;
	//Minimum size of bounding box (1*1*1)
	const int MIN_SIZE = 1;
	const int MAX_LEVEL = 5;
	//Objects waiting to be inserted
	static std::vector<Physical*> mPendingInsertion;
	//Used to check which nodes are active (eg 00000001 -> the first node (top-front-left) is active)
	typedef std::bitset<8> BYTE;
	unsigned char mActiveNodes;
	//Frames to wait until we insert
	int mMaxLifespan;
	int mCurLife;
	//Bools to tell if tree is ready and built
	static bool mReady;	//If false there is insertions left and needs to be built again (Updates)
	static bool mBuilt;	//If false the tree was not built yet
};