#include "LUtil.h"
#include <iostream>

void runMainLoop(int val);
/*
Pre Condition:
-Initialized freeGLUT
Post Condition:
-Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
Side Effects:
-Sets glutTimerFunc
*/

int main(int argc, char* args[])
{
	//Initialize FreeGLUT
	glutInit(&argc, args);

	//Create OpenGL 2.1 context
	glutInitContextVersion(3, 0);

	//Create Double Buffered Window
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("OpenGL");

	//Do post window/context creation initialization
	if (!initGL())
	{
		printf("Unable to initialize graphics library!\n");
		return 1;
	}

	//Set rendering function
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutSpecialFunc(processSpecialKeys);

	//Set main loop
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop(int val)
{
	//Frame logic
	update();
	render();

	//Run frame one more time
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
}
