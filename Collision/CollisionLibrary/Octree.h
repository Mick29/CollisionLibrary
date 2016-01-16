#pragma once

#include "Vector.h"
#include "AABB.h"
#include "GameObject.h"
#include <vector>
#include <queue>

class Octree {
public:
	Octree(AABB bounding, std::vector<GameObject*> objects);
	Octree(AABB bounding);
	Octree();
	~Octree();

	void buildTree();
	void renderTree();
	void updateTree(unsigned int dt);
	void updateTree();
	void insertObject(GameObject* obj);

	Octree* createNode(AABB region, std::vector<GameObject*> objects);
	Octree* createNode(AABB region, GameObject* object);

	void add(std::vector<GameObject*> objects);
	void add(GameObject* object);
private:
	int getCurrentLife();

	Octree* getParent();
	//Defines bounds for region
	AABB mBounds;
	//Children of the octree
	Octree* mNodes[8];
	//Parent reference
	Octree* mParent;
	//Store objects
	std::vector<GameObject*> mObjects;
	//Minimum size of bounding box (1*1*1)
	const int MIN_SIZE = 1;
	//Objects waiting to be inserted
	static std::queue<GameObject*> mPendingInsertion;
	//Used to check which nodes are active (eg 00000001 -> the first node (top-front-left) is active)
	uint8_t mActiveNodes;
	//Frames to wait until we insert
	int mMaxLifespan;
	int mCurLife;
	//Bools to tell if tree is ready and built
	static bool mReady;	//If false there is insertions left and needs to be built again (Updates)
	static bool mBuilt;	//If false the tree was not built yet
};