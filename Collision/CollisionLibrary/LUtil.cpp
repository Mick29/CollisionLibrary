/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

#include "LUtil.h"
#include "GLShapes.h"
#include "CollisionAPI.h"
#include "Player3D.h"
#include "Player2D.h"
#include <set>
#include <algorithm>

Octree* test;
Quadtree* test2d;

float x = 1.0f, z = 1.0f, lx = 0.0f, lz = 2.f, angle = 0.0f, y = -100.0f, xOrigin = 0.0f, deltaAngle = 0.0f, ly = 0.0f;

float newY;

bool dir = true, renderTree = true;

std::vector<Physical*> objects;
std::vector<Physical2D*> objects2d;

AABB gameWorld = AABB(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
AABB2D gameWorld2d = AABB2D(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));

AABB queryCube = AABB(Vec3(-5000.0f, 0.0f, 0.0f), Vec3(5000.0f, 10000.0f, 10000.0f));
AABB2D queryRect = AABB2D(Vec2(-200, 0), Vec2(200, 500));

int mode;

Player3D* player = NULL;
Player2D* player2d = NULL;

//Camera position
GLfloat gCameraX = 0.f;
GLfloat gCameraY = 0.f;

bool initGL2D() {
	mode = 0;
	for (int i = 0; i < 10; i++) {
		float xp = 0 + (rand() * (int)(SCREEN_WIDTH - 0) / RAND_MAX);
		float yp = (0 + (rand() * (int)(SCREEN_HEIGHT - 0) / RAND_MAX));
		Physical2D* o = new Physical2D(Vec2(xp, yp), AABB2D(Vec2(0, 0), Vec2(32, 32)));
		//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
		o->id = i;
		o->mStatic = true;
		objects2d.push_back(o);
	}

	//Physical2D* o1 = new Physical2D(Vec2(0, 0), AABB2D(Vec2(0, 0), Vec2(10, 10)));
	//Physical2D* o2 = new Physical2D(Vec2(260, 260), AABB2D(Vec2(0, 0), Vec2(10, 10)));
	//Physical2D* o3 = new Physical2D(Vec2(260, 0), AABB2D(Vec2(0, 0), Vec2(10, 10)));
	//Physical2D* o4 = new Physical2D(Vec2(0, 260), AABB2D(Vec2(0, 0), Vec2(10, 10)));

	//objects2d.push_back(o1);
	//objects2d.push_back(o2);
	//objects2d.push_back(o3);
	//objects2d.push_back(o4);

	test2d = new Quadtree(gameWorld2d, objects2d);

	player2d = new Player2D();
	player2d->init();
	player2d->setPosition(Vec2(0, 0));
	player2d->id = -1;
	test2d->insertObject(player2d);

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	return true;
}

bool initGL()
{
	mode = 1;
	for (int i = 0; i < 10; i++) {
		float xp = 0 + (rand() * (int)(10000 - 0) / RAND_MAX);
		float yp = 0 + (rand() * (int)(10000 - 0) / RAND_MAX);
		float zp = 0 + (rand() * (int)(-10000 - 0) / RAND_MAX);
		GameObject* o = new GameObject(Vec3(xp, yp, zp), AABB(Vec3(0, 0, -500), Vec3(500, 100, 0)));
		//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
		o->id = i;
		objects.push_back(o);
	}

	//int collisionCheck = 0;

	//for (Physical* obj1 : objects) {
	//	for (Physical* obj2 : objects) {
	//		//if (obj1 == obj2)
	//		//	continue;

	//		if (obj1->getBoundingBox().intersect(obj2->getBoundingBox())) {
	//			printf("COLLISION");
	//		}
	//		collisionCheck++;
	//	}
	//}

	//Physical* o1 = new Physical(Vec3(0, 0, 0), AABB(Vec3(0, 0, 0), Vec3(100, 100, 100)));
	//o1->id = 2;
	//Physical* o2 = new Physical(Vec3(50, 50, 50), AABB(Vec3(0, 0, 0), Vec3(100, 100, 100)));
	//o2->id = 3;

	//objects.push_back(o1);
	//objects.push_back(o2);

	test = new Octree(gameWorld, objects);
	
	//Ray testRay = Ray(Vec3(0, 0, 0), Vec3(100, 0, 0));
	//Physical* o = new Physical(Vec3(25, 0, 0), AABB(Vec3(0, 0, 0), Vec3(5, 5, 5)));

	//if (o->getBoundingBox().intersect(testRay)) {
	//	printf("Length of ray: %f", o->getBoundingBox().getRayLength());
	//}

	player = new Player3D();
	player->init();
	player->setPosition(Vec3(50, 1000, -50));
	player->id = -1;
	test->insertObject(player);

	GameObject* o = new GameObject();
	o->setPosition(Vec3(0, 0, 0));
	o->setBoundingBox(AABB(Vec3(0, 0, -10000), Vec3(10000, 100, 0)));
	o->id = 0;
	test->insertObject(o);

	//GameObject* o2 = new GameObject();
	//o2->setPosition(Vec3(500, 100, 500));
	//o2->setBoundingBox(AABB(Vec3(0, 0, 0), Vec3(100, 100, 100)));
	//test->insertObject(o2);

	objects.push_back(o);
	//objects.push_back(o2);

	test->buildTree();

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		Sleep(5000);
		return false;
	}

	return true;
}

void update2D() {
	player2d->update(1000 / SCREEN_FPS);

	test2d->updateTree(1000 / SCREEN_FPS);

	if (!test2d->isCollisionEnabled()) {
		AABB2D region = player2d->getBoundingBox();
		std::vector<Physical2D*> objectsTest = test2d->checkQuery(region);
		std::set<Physical2D*> objectsReduce(objectsTest.begin(), objectsTest.end());

		for (Physical2D* obj : objectsReduce) {
			if (obj != player2d) {
				if (obj->getBoundingBox().intersect(player2d->getBoundingBox())) {
					player2d->handleCollision(obj);
				}
			}
		}
	}

	//if (queryRect.mMax.x < 10000.0f) {
	//	queryRect.mMin.x += 20.0f;
	//	queryRect.mMax.x += 20.0f;

	//	std::vector<Physical2D*> queryObjects;
	//	queryObjects = test2d->checkQuery(queryRect);

	//	for (int i = 0; i < queryObjects.size(); i++) {
	//		Physical2D* obj = queryObjects.at(i);
	//		obj->mAlive = false;
	//		objects2d.erase(std::remove(objects2d.begin(), objects2d.end(), obj), objects2d.end());
	//	}

	//	queryObjects.clear();
	//}

	for (Physical2D* obj : objects2d) {
		obj->update(1000 / SCREEN_FPS);
	}
}

float getDistance(Vec3 p1, Vec3 p2) {
	return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)) + ((p2.z - p1.z) * (p2.z - p1.z)));
}

void update()
{
	for (Physical* obj : objects) {
		obj->update(1000 / SCREEN_FPS);
	}

	player->update(1000 / SCREEN_FPS);

	test->updateTree(1000 / SCREEN_FPS);

	if (!test->isCollisionEnabled()) {
		AABB region = player->getBoundingBox();
		std::vector<Physical*> objectsTest = test->checkQuery(region);
		std::set<Physical*> objectsReduce(objectsTest.begin(), objectsTest.end());

		for (Physical* obj : objectsReduce) {
			if (obj != player) {
				if (obj->getBoundingBox().intersect(player->getBoundingBox())) {
					player->handleCollision(obj);
				}
			}
		}
	}

	//x = player->getPosition().x + (cos(45) * getDistance(Vec3(x, y, z), player->getPosition())) - 500;
	//y = player->getPosition().y + (cos(45) * getDistance(Vec3(x, y, z), player->getPosition())) - 100;
	//z = player->getPosition().z + (-cos(90) * getDistance(Vec3(x, y, z), player->getPosition())) - 100;
	//if (y >= 100) {
	//	dir = false;
	//}
	//if (y <= 0) {
	//	dir = true;
	//}

	//if (dir) y += 1.0f;
	//else y -= 1.0f;

	//if (queryCube.mMax.x < 10000.0f) {
	//	queryCube.mMin.x += 20.0f;
	//	queryCube.mMax.x += 20.0f;

	//	std::vector<Physical*> queryObjects;
	//	queryObjects = test->checkQuery(queryCube);

	//	for (int i = 0; i < queryObjects.size(); i++) {
	//		Physical* obj = queryObjects.at(i);
	//		obj->mAlive = false;
	//		objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
	//	}

	//	queryObjects.clear();
	//}

	//y += newY;

	//newY = 0;
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();
	//gluLookAt(x, y, z, x + lx, y + ly, lz + z, 0.0f, 1.0f, 0.0f);

	//gluLookAt(x, y, z, player->getPosition().x, player->getPosition().y, player->getPosition().z, 0.0f, 1.0f, 0.0f);
	Vec3 offset(0, 100, 1000);
	Vec3 cameraPosition = player->getPosition() + offset;

	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		player->getPosition().x,player->getPosition().y, player->getPosition().z,
		0.0f, 1.0f, 0.0f);

	// Draw ground
	if(renderTree)
		test->renderTree();

	player->render();

	glColor3f(0xFF, 0x00, 0x00);
	glTranslatef(0.f, 0.f, 0.f);
	glutSolidSphere(20.0f, 100.0f, 100.0f);

	glColor3f(0xFF, 0xFF, 0xFF);
	//queryCube.renderWireframe();

	for (Physical* obj : objects) {
		int colorID = obj->id % 5;
		if(colorID == 0)
			glColor3f(0xFF, 0x00, 0xFF);
		else if (colorID == 1)
			glColor3f(0x00, 0x00, 0xFF);
		else if (colorID == 2)
			glColor3f(0xFF, 0x00, 0x00);
		else if (colorID == 3)
			glColor3f(0x00, 0xFF, 0x00);
		else if (colorID == 4)
			glColor3f(0xFF, 0xFF, 0x00);

		if(obj->id == -1 || obj->mCollisionDetected)
			glColor3f(0xFF, 0xFF, 0xFF);

		//glTranslatef(obj->getPosition().x, obj->getPosition().y, obj->getPosition().z);
		//glutSolidSphere(1.0f, 100.0f, 100.0f);
		if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax()) {
			obj->getBoundingBox().renderSolid();
		}
		else {
			glutSolidSphere(obj->getBoundingSphere().getRadius(), 100.0f, 100.0f);
		}
	}

	glPopMatrix();

	//Update screen
	glutSwapBuffers();
}

void render2D() {
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();

	gluLookAt(x, z, 0, x, z, -1.0, 0.0f, 1.0f, 0.0f);

	//gluLookAt(player2d->getPosition().x, player2d->getPosition().y, 0.0f,
	//	offset.x, offset.y, -1.0f,
	//	0.0f, 1.0f, 0.0f);

	//glPopMatrix();

	if(renderTree)
		test2d->renderTree();

	player2d->render();

	for (Physical2D* obj : objects2d) {
		int colorID = obj->id % 5;
		if (colorID == 0)
			glColor3f(0xFF, 0x00, 0xFF);
		else if (colorID == 1)
			glColor3f(0x00, 0x00, 0xFF);
		else if (colorID == 2)
			glColor3f(0xFF, 0x00, 0x00);
		else if (colorID == 3)
			glColor3f(0x00, 0xFF, 0x00);
		else if (colorID == 4)
			glColor3f(0xFF, 0xFF, 0x00);

		if (obj->id == -1)
			glColor3f(0xFF, 0xFF, 0xFF);

		obj->getBoundingBox().render();
	}

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(75.0f, aspect, 0.1f, 50000.0f);
}

void reshape2D(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar

	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);
}

void Keyboard(unsigned char key, int x, int y) {
	if (player != NULL)
		player->handleInput(key, x, y);

	if (player2d != NULL)
		player2d->handleInput(key, x, y);

	switch (key) {
	case 'q': {
		if (mode == 1) {
			AABB bounds = test->getBounds();
			float xp = (rand() * (int)(bounds.getMax().x) / RAND_MAX) - (int)bounds.getMin().x;
			float yp = (rand() * (int)(bounds.getMax().y) / RAND_MAX) - (int)bounds.getMin().y;
			float zp = (rand() * (int)(-bounds.getMax().z) / RAND_MAX) - (int)bounds.getMin().z;
			Physical* o = new Physical(Vec3(xp, yp, zp), AABB(Vec3(0, 0, -500), Vec3(1000, 100, 0)));
			//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
			objects.push_back(o);
			test->insertObject(o);
			o->id = objects.size();
		}
		else {
			float xp = (rand() * (int)(SCREEN_WIDTH - 0) / RAND_MAX);
			float yp = (rand() * (int)(SCREEN_HEIGHT - 0) / RAND_MAX);
			Physical2D* o = new Physical2D(Vec2(xp, yp), AABB2D(Vec2(0, 0), Vec2(10, 10)));
			//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
			objects2d.push_back(o);
			test2d->insertObject(o);
			o->id = objects2d.size();
			o->mStatic = true;
		}
	}
		break;
	//case 'q':
	//	newY += 100.f;
	//	break;
	//case 'w':
	//	newY -= 100.f;
	//	break;
	//case 'e':
	//	angle -= 0.2f;
	//	ly = cos(angle);
	//	break;
	//case 'r':
	//	angle += 0.2f;
	//	ly = cos(angle);
	//	break;
	case 'e':
		if (renderTree)
			renderTree = false;
		else
			renderTree = true;
		break;
	case 'b':
		if (mode == 1)
			test->buildTree();
		else
			test2d->buildTree();
		break;
	case 'c': {
		if (mode == 1) {
			std::vector<Physical*> queryObjects;
			queryObjects = test->checkQuery(queryCube);

			for (int i = 0; i < queryObjects.size(); i++) {
				Physical* obj = queryObjects.at(i);
				obj->id = -1;
				obj->mAlive = false;

				objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
			}

			queryObjects.clear();
		}
		break;
	}
	case 'p':
		if (mode == 1) {
			printf("Number of objects: %i\n", objects.size() + 1);
			test->printDetails();
		}
		else {
			printf("Number of objects: %i\n", objects2d.size() + 1);
			test2d->printDetails();
		}
		break;
	case 'v':
		if (mode == 1) {
			test->toggleCollision();
		}
		else {
			test2d->toggleCollision();
		}
		break;
	//case 'r':
	//	if (mode == 1) {
	//		test->resize(AABB(Vec3(-10000, -10000, -10000), Vec3(1000000, 1000000, 1000000)));
	//	}

	//	break;
	}

	glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y) {
	if(player != NULL)
		player->handleInputUp(key, x, y);

	if (player2d != NULL)
		player2d->handleInputUp(key, x, y);

	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 50.0f;

	if(player != NULL)
		player->handleInputSpecial(key, xx, yy);

	if (player2d != NULL)
		player2d->handleInputSpecial(key, xx, yy);

	//if (mode == 1) {
	//	switch (key) {
	//	case GLUT_KEY_LEFT:
	//		angle -= 0.2f;
	//		lx = sin(angle);
	//		lz = -cos(angle);
	//		break;
	//	case GLUT_KEY_RIGHT:
	//		angle += 0.2f;
	//		lx = sin(angle);
	//		lz = -cos(angle);
	//		break;
	//	case GLUT_KEY_UP:
	//		x += lx * fraction;
	//		z += lz * fraction;
	//		break;
	//	case GLUT_KEY_DOWN:
	//		x -= lx * fraction;
	//		z -= lz * fraction;
	//		break;
	//	}
	//}
	//else {
		switch (key) {
		case GLUT_KEY_LEFT:
			x -= lx * fraction;
			break;
		case GLUT_KEY_RIGHT:
			x += lx * fraction;
			break;
		case GLUT_KEY_UP:
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			z -= lz * fraction;
			break;
		}
	//}

		glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		if (mode == 0) {
			if (state == GLUT_UP) {
				Physical2D* o = new Physical2D(Vec2(x, y), AABB2D(Vec2(0, 0), Vec2(100, 10)));
				objects2d.push_back(o);
				test2d->insertObject(o);
				o->id = objects2d.size();
				o->mStatic = true;
			}
		}
		else {
			// when the button is released
			if (state == GLUT_UP) {
				angle += deltaAngle;
				xOrigin = -1;
			}
			else {// state = GLUT_DOWN
				xOrigin = x;
			}
		}
	}
}

void mouseMove(int x, int y) {

	if (mode == 1) {
		// this will only be true when the left button is down
		if (xOrigin >= 0) {

			// update deltaAngle
			deltaAngle = (x - xOrigin) * 0.001f;

			// update camera's direction
			lx = sin(angle + deltaAngle);
			lz = -cos(angle + deltaAngle);
		}
	}
}