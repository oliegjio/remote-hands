#include "shape.h"

#include <iostream>

shape *shape::make_cube()
{
	auto cube = new shape;

	cube->vertices.reserve(24);

	cube->vertices.push_back(vertex(1.0f, 1.0f, -1.0f)); // Top Right Of The Quad (Top)
	cube->vertices.push_back(vertex(-1.0f, 1.0f, -1.0f)); // Top Left Of The Quad (Top)
	cube->vertices.push_back(vertex(-1.0f, 1.0f, 1.0f)); // Bottom Left Of The Quad (Top)
	cube->vertices.push_back(vertex(1.0f, 1.0f, 1.0f)); // Bottom Right Of The Quad (Top)

	cube->vertices.push_back(vertex(1.0f, -1.0f, 1.0f)); // Top Right Of The Quad (Bottom)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, 1.0f)); // Top Left Of The Quad (Bottom)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, -1.0f)); // Bottom Left Of The Quad (Bottom)
	cube->vertices.push_back(vertex(1.0f, -1.0f, -1.0f)); // Bottom Right Of The Quad (Bottom)

	cube->vertices.push_back(vertex(1.0f, 1.0f, 1.0f)); // Top Right Of The Quad (Front)
	cube->vertices.push_back(vertex(-1.0f, 1.0f, 1.0f)); // Top Left Of The Quad (Front)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, 1.0f)); // Bottom Left Of The Quad (Front)
	cube->vertices.push_back(vertex(1.0f, -1.0f, 1.0f)); // Bottom Right Of The Quad (Front)

	cube->vertices.push_back(vertex(1.0f, -1.0f, -1.0f)); // Top Right Of The Quad (Back)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, -1.0f)); // Top Left Of The Quad (Back)
	cube->vertices.push_back(vertex(-1.0f, 1.0f, -1.0f)); // Bottom Left Of The Quad (Back)
	cube->vertices.push_back(vertex(1.0f, 1.0f, -1.0f)); // Bottom Right Of The Quad (Back)

	cube->vertices.push_back(vertex(-1.0f, 1.0f, 1.0f)); // Top Right Of The Quad (Left)
	cube->vertices.push_back(vertex(-1.0f, 1.0f, -1.0f)); // Top Left Of The Quad (Left)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, -1.0f)); // Bottom Left Of The Quad (Left)
	cube->vertices.push_back(vertex(-1.0f, -1.0f, 1.0f)); // Bottom Right Of The Quad (Left)

	cube->vertices.push_back(vertex(1.0f, 1.0f, -1.0f)); // Top Right Of The Quad (Right)
	cube->vertices.push_back(vertex(1.0f, 1.0f, 1.0f)); // Top Left Of The Quad (Right)
	cube->vertices.push_back(vertex(1.0f, -1.0f, 1.0f)); // Bottom Left Of The Quad (Right)
	cube->vertices.push_back(vertex(1.0f, -1.0f, -1.0f)); // Bottom Right Of The Quad (Right)

	return cube;
}

void shape::draw() const
{
	glBegin(GL_QUADS);
	for (const auto & vertex : vertices)
	{
		glVertex3f(vertex.x(), vertex.y(), vertex.z());
	}
	glEnd();
}

void shape::translate(const GLfloat & x, const GLfloat & y, const GLfloat & z)
{
	vertex translation(x, y, z);
	for (auto & v : vertices)
	{
		v += translation;
	}
}

void shape::print() const
{
	for (const auto & vertex : vertices)
	{
		vertex.print();
	}
}