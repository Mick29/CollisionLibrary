#pragma once

#include "Vector.h"
#include "AABB.h"
#include "Physical.h"
#include <vector>
#include "CollisionResult.h"

class Octree {
public:
	//Constructors & destructor
	Octree(AABB bounding, std::vector<Physical*> objects);
	Octree(AABB bounding);
	Octree();
	~Octree();

	//Builds the tree
	void buildTree();
	
	//Renders the tree
	void renderTree();

	//Udates the tree
	void updateTree(unsigned int dt);

	//Not used
	void updateTree();

	//Insert object into the tree
	void insertObject(Physical* obj);

	//Creates a tree node
	Octree* createNode(AABB region, std::vector<Physical*> objects);
	Octree* createNode(AABB region, Physical* object);

	//Not used
	void add(std::vector<Physical*> objects);
	void add(Physical* object);

	//Get the trees root
	Octree* getRoot();

	//Get this nodes bounds
	AABB getBounds();

	//Returns whether node has children nodes
	bool hasChildren();

	//Gets a list of collisions
	std::vector<CollisionResult*> getIntersection(std::vector<Physical*> objects);

	//Prints the tree (Not working)
	void printTree();

	//Get the level this nodes at
	int getLevel();

	//Get amount of objects (duplicate == true then duplicate objects are counted)
	int getObjectSize(bool duplicate = true);

	//Get objects in a certain region
	std::vector<Physical*> checkQuery(AABB region);

	//Resizes the tree
	void resize(AABB region);

	//Gets all the objects in the tree
	std::vector<Physical*> getAllObjects();

	//Prints details on the tree
	void printDetails();

	//Whether collision checking is enabled by default
	bool isCollisionEnabled();

	//Enable collision detection
	void toggleCollision();
private:
	//Gets the current life of the node
	int getCurrentLife();

	//Gets the parent node
	Octree* getParent();

	//Finds a new boundary for the tree
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
	const int MAX_LEVEL = 5; //Not used

	//Objects waiting to be inserted
	static std::vector<Physical*> mPendingInsertion;
	static std::vector<Physical*> mOutboundObjects;

	//Used to check which nodes are active (eg 00000001 -> the first node (top-front-left) is active)
	unsigned char mActiveNodes;

	//Frames to wait until we insert
	int mMaxLifespan;
	int mCurLife;

	//Use octrees built in check collision function
	bool mBuiltInCheck;

	//Bools to tell if tree is ready and built
	static bool mReady;	//If false there is insertions left and needs to be built again (Updates)
	static bool mBuilt;	//If false the tree was not built yet
	static bool mNeedsResize;  //If there is objects outside of tree then this is set
	static bool mPrintChecks;  //If we want to print collision checks then this is set
	static int mCollisionChecks;  //Counts number of collision checks for comparing to brute force
};