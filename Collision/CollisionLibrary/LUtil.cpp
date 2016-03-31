/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

#include "LUtil.h"
#include "GLShapes.h"
#include "Ray.h"
#include "Quadtree.h"

Octree* test;
Quadtree* test2d;

float x = 0.0f, z = 5.f, lx = 0.0f, lz = 2.f, angle = 0.0f, y = 0.0f, xOrigin = 0.0f, deltaAngle = 0.0f, ly = 0.0f;
float newY;
bool dir = true, renderTree = true;
std::vector<Physical*> objects;
std::vector<Physical2D*> objects2d;
AABB gameWorld = AABB(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
AABB2D gameWorld2d = AABB2D(Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));
AABB queryCube = AABB(Vec3(-5000.0f, 0.0f, 0.0f), Vec3(5000.0f, 10000.0f, 10000.0f));
AABB2D queryRect = AABB2D(Vec2(-200, 0), Vec2(200, 500));

int mode;

//Camera position
GLfloat gCameraX = 0.f;
GLfloat gCameraY = 0.f;

bool initGL2D() {
	mode = 1;
	for (int i = 0; i < 10000; i++) {
		float xp = 0 + (rand() * (int)(SCREEN_WIDTH - 0) / RAND_MAX);
		float yp = (0 + (rand() * (int)(SCREEN_HEIGHT - 0) / RAND_MAX));
		Physical2D* o = new Physical2D(Vec2(xp, yp), AABB2D(Vec2(0, 0), Vec2(1, 1)));
		//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
		o->id = i;
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
	for (int i = 0; i < 1000; i++) {
		float xp = 0 + (rand() * (int)(10000 - 0) / RAND_MAX);
		float yp = 0 + (rand() * (int)(10000 - 0) / RAND_MAX);
		float zp = 0 + (rand() * (int)(10000 - 0) / RAND_MAX);
		Physical* o = new Physical(Vec3(xp, yp, zp), AABB(Vec3(0, 0, 0), Vec3(100, 100, 100)));
		//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
		o->id = i;
		objects.push_back(o);
	}

	test = new Octree(gameWorld, objects);
	
	//Ray testRay = Ray(Vec3(0, 0, 0), Vec3(100, 0, 0));
	//Physical* o = new Physical(Vec3(25, 0, 0), AABB(Vec3(0, 0, 0), Vec3(5, 5, 5)));

	//if (o->getBoundingBox().intersect(testRay)) {
	//	printf("Length of ray: %f", o->getBoundingBox().getRayLength());
	//}

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
	test2d->updateTree(1000 / SCREEN_FPS);

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

void update()
{
	test->updateTree(1000 / SCREEN_FPS);

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

	y += newY;

	newY = 0;

	for (Physical* obj : objects) {
		obj->update(1000 / SCREEN_FPS);
	}
}

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, lz + z, 0.0f, 1.0f, 0.0f);

	// Draw ground
	if(renderTree)
		test->renderTree();

	glColor3f(0xFF, 0x00, 0x00);
	glTranslatef(0.f, 0.f, 0.f);
	glutSolidSphere(20.0f, 100.0f, 100.0f);

	glColor3f(0xFF, 0xFF, 0xFF);
	queryCube.renderWireframe();

	for (Physical* obj : objects) {
		glPushMatrix();
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

		if(obj->id == -1)
			glColor3f(0xFF, 0xFF, 0xFF);

		glTranslatef(obj->getPosition().x, obj->getPosition().y, obj->getPosition().z);
		//glutSolidSphere(1.0f, 100.0f, 100.0f);
		if (obj->getBoundingBox(false).getMin() != obj->getBoundingBox(false).getMax()) {
			AABB temp = obj->getBoundingBox(false);
			GLRectangle rect(Vec3(temp.mMax.x, temp.mMax.y, temp.mMax.z), false);
			rect.render();
		}
		else {
			glutSolidSphere(obj->getBoundingSphere().getRadius(), 100.0f, 100.0f);
		}
		glPopMatrix();
	}

	//Update screen
	glutSwapBuffers();
}

void render2D() {
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();
	gluLookAt(x, z, 0, x, z, -1.0, 0.0f, 1.0f, 0.0f);

	//glPushMatrix();
	if(renderTree)
		test2d->renderTree();

	//glPopMatrix();

	for (Physical2D* obj : objects2d) {
		glPushMatrix();
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

		glTranslatef(obj->getPosition().x + gCameraX, obj->getPosition().y + gCameraY, 0.0f);
		//glutSolidSphere(1.0f, 100.0f, 100.0f);
		//if (obj->getBoundingBox(false).mMin != obj->getBoundingBox(false).mMax) {
			AABB2D temp = obj->getBoundingBox(false);
			temp.render();
		//}
		glPopMatrix();
	}

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
	gluPerspective(90.0f, aspect, 0.1f, 500000.0f);
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
	switch (key) {
	case 'a': {
		if (mode == 1) {
			float xp = 0 + (rand() * (int)(1000 - 0) / RAND_MAX);
			float yp = 0 + (rand() * (int)(1000 - 0) / RAND_MAX);
			float zp = 0 + (rand() * (int)(1000 - 0) / RAND_MAX);
			Physical* o = new Physical(Vec3(xp, yp, zp), AABB(Vec3(0, 0, 0), Vec3(10, 10, 10)));
			//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
			objects.push_back(o);
			test->insertObject(o);
			o->id = objects.size();
		}
		else {
			float xp = 0 + (rand() * (int)(SCREEN_WIDTH - 0) / RAND_MAX);
			float yp = 0 + (rand() * (int)(SCREEN_HEIGHT - 0) / RAND_MAX);
			Physical2D* o = new Physical2D(Vec2(xp, yp), AABB2D(Vec2(0, 0), Vec2(1, 1)));
			//Physical* o = new Physical(Vec3(xp, yp, zp), BoundingSphere(50, Vec3(xp, yp, zp)));
			objects2d.push_back(o);
			test2d->insertObject(o);
			o->id = objects.size();
		}
	}
		break;
	case 'q':
		newY += 100.f;
		break;
	case 'w':
		newY -= 100.f;
		break;
	case 'e':
		angle -= 0.2f;
		ly = cos(angle);
		break;
	case 'r':
		angle += 0.2f;
		ly = cos(angle);
		break;
	case 'd':
		if (renderTree)
			renderTree = false;
		else
			renderTree = true;
		break;
	case 'b':
		test->buildTree();
		//test->updateTree();
		break;
	case 'c': {
		std::vector<Physical*> queryObjects;
		queryObjects = test->checkQuery(queryCube);

		for (int i = 0; i < queryObjects.size(); i++) {
			Physical* obj = queryObjects.at(i);
			obj->id = -1;
			obj->mAlive = false;
			objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
		}

		queryObjects.clear();
		break;
	}
	case 'p':
		/*queryCube.mMax = Vec3(250.0f, 500.0f, 1000.f);
		queryCube.mMin = Vec3(0.0f, 0.0f, 0.0f);*/
		printf("Number of objects: %i\n", objects2d.size());
		printf("Number of objects in tree: %i\n", test2d->getObjectSize());
		break;
	}

	////If the user pressed w/a/s/d, change camera position
	//if (key == 'u')
	//{
	//	//gCameraY -= 16.f;
	//	y -= 16;
	//}
	//else if (key == 'j')
	//{
	//	//gCameraY += 16.f;
	//	y += 16;
	//}
	//else if (key == 'h')
	//{
	//	//gCameraX -= 16.f;
	//	x -= 16;
	//}
	//else if (key == 'k')
	//{
	//	//gCameraX += 16.f;
	//	x += 16;
	//}

	//////Take saved matrix off the stack and reset it
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	//glLoadIdentity();

	////Move camera to position
	////glTranslatef(-gCameraX, -gCameraY, 0.f);
	////glOrtho(gCameraX, gCameraX + SCREEN_WIDTH, gCameraY + SCREEN_HEIGHT, gCameraY, 1.0, -1.0);

	////Save default matrix again with camera translation
	//glPushMatrix();
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 50.0f;

	if (mode == 1) {
		switch (key) {
		case GLUT_KEY_LEFT:
			angle -= 0.2f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT:
			angle += 0.2f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP:
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * fraction;
			z -= lz * fraction;
			break;
		}
	}
	else {
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
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

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

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}