#include "vertex.h"

#include <iostream>

vertex::vertex(const GLfloat & x, const GLfloat & y, const GLfloat & z)
{
	data.reserve(3);
	data.push_back(x);
	data.push_back(y);
	data.push_back(z);
}

vertex vertex::operator+(const vertex & other) const
{
	return vertex(x() + other.x(), y() + other.y(), z() + other.z());
}

vertex vertex::operator*(const vertex & other) const
{
	return vertex(x() * other.x(), y() * other.y(), z() * other.z());
}

vertex &vertex::operator+=(const vertex & other)
{
	data[0] += other.x();
	data[1] += other.y();
	data[2] += other.z();
	return *this;
}

vertex &vertex::operator*=(const vertex & other)
{
	data[0] *= other.x();
	data[1] *= other.y();
	data[2] *= other.z();
	return *this;
}

bool vertex::operator==(const vertex & other) const
{
	if (x() == other.x() ||
		y() == other.y() ||
		z() == other.z())
	{
		return true;
	}
	else
	{
		return false;
	}
}

GLfloat vertex::x() const
{
	return data[0];
}

void vertex::x(const GLfloat & x)
{
	data[0] = x;
}

GLfloat vertex::y() const
{
	return data[1];
}

void vertex::y(const GLfloat & y)
{
	data[1] = y;
}

GLfloat vertex::z() const
{
	return data[2];
}

void vertex::z(const GLfloat & z)
{
	data[2] = z;
}

void vertex::print() const
{
	std::cout << " (" << data[0] << ", " << data[1] << ", " << data[2] << ") ";
}
