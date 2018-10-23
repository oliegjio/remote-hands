#pragma once

#include <GL/GL.h>

class scene_object {
public:
	virtual void draw() const = 0;
	virtual void rotate(const GLfloat &angle) = 0;
	virtual void translate(const GLfloat &x, const GLfloat &y, const GLfloat &z) = 0;
	virtual void scale(const GLfloat &x, const GLfloat &y, const GLfloat &z) = 0;
};