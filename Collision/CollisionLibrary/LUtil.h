#pragma once

#include "LOpenGL.h"
#include "Octree.h"
#include <stdio.h>

//Screen Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;

extern GLfloat gCameraX, gCameraY;
extern bool renderTree;

bool initGL();
bool initGL2D();
/*
Pre Condition:
-A valid OpenGL context
Post Condition:
-Initializes matrices and clear color
-Reports to console if there was an OpenGL error
-Returns false if there was an error in initialization
Side Effects:
-Projection matrix is set to identity matrix
-Modelview matrix is set to identity matrix
-Matrix mode is set to modelview
-Clear color is set to black
*/

void update();
void update2D();
/*
Pre Condition:
-None
Post Condition:
-Does per frame logic
Side Effects:
-None
*/

void render();
void render2D();
/*
Pre Condition:
-A valid OpenGL context
-Active modelview matrix
Post Condition:
-Renders the scene
Side Effects:
-Clears the color buffer
-Swaps the front/back buffer
*/

// Derived class keyboard
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);

void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);

void processSpecialKeys(int key, int xx, int yy);
void updateCamera();
void reshape(GLsizei width, GLsizei height);
void reshape2D(GLsizei width, GLsizei height);