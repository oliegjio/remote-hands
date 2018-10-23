#pragma once

#include <gl/GL.h>
#include <vector>

class vertex {
public:
	vertex(const GLfloat & x, const GLfloat & y, const GLfloat & z);

	vertex operator+(const vertex & other) const;
	vertex operator*(const vertex & other) const;

	vertex & operator+=(const vertex & other);
	vertex & operator*=(const vertex & other);

	bool operator==(const vertex & other) const;

	GLfloat x() const;
	void x(const GLfloat & x);

	GLfloat y() const;
	void y(const GLfloat & y);

	GLfloat z() const;
	void z(const GLfloat & z);

	void print() const;

private:
	std::vector<GLfloat> data;
};