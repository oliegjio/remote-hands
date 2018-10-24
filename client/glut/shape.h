#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/glut.h>

#include "vec.h"

class shape {
public:
	static shape *make_cube();

	void draw() const;
	void print() const;

	vec4 rotation { 0.0f, 0.0f, 0.0f, 0.0f };
	vec3 translation { 0.0f, 0.0f, 0.0f };
	vec3 scaling { 1.0f, 1.0f, 1.0f };
	vec3 color { 0.0f, 0.0f, 0.0f };

private:
	shape() {};

    std::vector<vec3> vertices;

	//vec3 center{ 0.0f, 0.0f, 0.0f };
	//void compute_center();
};