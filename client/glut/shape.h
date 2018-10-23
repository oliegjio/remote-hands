#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif
#include <gl/glut.h>

#include "vertex.h"
#include "scene_object.h"

class shape : public scene_object {
public:
	static shape *make_cube();

	void draw() const;
	void rotate(const GLfloat & angle);
	void translate(const GLfloat & x, const GLfloat & y, const GLfloat & z);
	void scale(const GLfloat & x, const GLfloat & y, const GLfloat & z);

	void print() const;

private:
	std::vector<vertex> vertices;

	shape();
};