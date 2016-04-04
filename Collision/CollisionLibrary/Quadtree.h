#pragma once

#include "Vector.h"
#include "AABB.h"
#include "Physical.h"
#include <vector>
#include "CollisionResult.h"

class Quadtree {
public:
	//Constructors & destructor
	Quadtree(AABB2D bounding, std::vector<Physical2D*> objects);
	Quadtree(AABB2D bounding);
	Quadtree();
	~Quadtree();

	//Builds the tree
	void buildTree();

	//Renders the tree
	void renderTree();

	//Udates the tree
	void updateTree(unsigned int dt);

	//Not used
	void updateTree();

	//Insert object into the tree
	void insertObject(Physical2D* obj);

	//Creates a tree node
	Quadtree* createNode(AABB2D region, std::vector<Physical2D*> objects);
	Quadtree* createNode(AABB2D region, Physical2D* object);

	//Not used
	void add(std::vector<Physical2D*> objects);
	void add(Physical2D* object);

	//Get the trees root
	Quadtree* getRoot();

	//Get this nodes bounds
	AABB2D getBounds();

	//Returns whether node has children nodes
	bool hasChildren();

	//Gets a list of collisions
	std::vector<CollisionResult2D*> getIntersection(std::vector<Physical2D*> objects);

	//Prints the tree (Not working)
	void printTree();

	//Get the level this nodes at
	int getLevel();

	//Get amount of objects (duplicate == true then duplicate objects are counted)
	int getObjectSize(bool duplicate = true);

	//Get objects in a certain region
	std::vector<Physical2D*> checkQuery(AABB2D region);

	//Resizes the tree
	void resize(AABB2D region);

	//Gets all the objects in the tree
	std::vector<Physical2D*> getAllObjects();

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
	Quadtree* getParent();

	//Finds a new boundary for the tree
	void findNewBoundary();
	void findNewCube();

	//Defines bounds for region
	AABB2D mBounds;

	//Children of the 
	Quadtree* mNodes[4];

	//Parent reference
	Quadtree* mParent;

	//Store objects
	std::vector<Physical2D*> mObjects;

	//Minimum size of bounding box (1*1*1)
	const int MIN_SIZE = 1;
	const int MAX_LEVEL = 5; //Not used

							 //Objects waiting to be inserted
	static std::vector<Physical2D*> mPendingInsertion;
	static std::vector<Physical2D*> mOutboundObjects;

	//Used to check which nodes are active (eg 00000001 -> the first node (top-front-left) is active)
	unsigned char mActiveNodes;

	//Frames to wait until we insert
	int mMaxLifespan;
	int mCurLife;

	//Use s built in check collision function
	bool mBuiltInCheck;

	//Bools to tell if tree is ready and built
	static bool mReady;	//If false there is insertions left and needs to be built again (Updates)
	static bool mBuilt;	//If false the tree was not built yet
	static bool mNeedsResize;  //If there is objects outside of tree then this is set
	static bool mPrintChecks;  //If we want to print collision checks then this is set
	static int mCollisionChecks;  //Counts number of collision checks for comparing to brute force
};