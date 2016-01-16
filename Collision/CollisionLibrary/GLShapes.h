#pragma once

#include "LOpenGL.h"
#include "Vector.h"

class GLRectangle {
public:
	GLRectangle(float width, float height, float depth, bool wireframe = true);
	GLRectangle(Vec3 dimensions, bool wireframe = true);

	void render();

	Vec3 getDimensions();
	void setPosition(Vec3 pos);
private:
	Vec3 mSize;
	Vec3 mPosition;

	//Render as wireframe
	bool bWireframe;
};