/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

#include "LUtil.h"
#include "GLShapes.h"

Octree* test;

float x = 0.0f, z = 5.f, lx = 0.0f, lz = 2.f, angle = 0.0f;

bool initGL()
{
	AABB gameWorld = AABB(Vec3(0.0f, 0.0f, 0.0f), Vec3(5.f, 5.f, 5.f));
	GameObject* o1 = new GameObject(Vec3(-2, 0, -2), AABB(Vec3(0, 0, 0), Vec3(1, 1, 1)));
	GameObject* o2 = new GameObject(Vec3(2, 0, 2), AABB(Vec3(0, 0, 0), Vec3(1, 1, 1)));
	
	std::vector<GameObject*> objects;
	objects.push_back(o1);
	objects.push_back(o2);

	test = new Octree(gameWorld, objects);

	test->buildTree();

	test->insertObject(o2);

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

void update()
{

}

void render()
{
	GLRectangle rect = GLRectangle(1, 1, 1);
	rect.setPosition(Vec3(0, 0, -7));
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();
	gluLookAt(x, 1.0f, z, x + lx, 1.0f, lz + z, 0.0f, 1.0f, 0.0f);

	// Draw ground
	test->renderTree();

	//Update screen
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
	gluPerspective(45.0f, aspect, 0.1f, 10000.0f);
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 2.0f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
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