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

	vec4 basis_x {1.0f, 0.0f, 0.0f, 1.0f};
    vec4 basis_y {0.0f, 1.0f, 0.0f, 1.0f};
    vec4 basis_z {0.0f, 0.0f, 1.0f, 1.0f};

private:
	shape() {};

    std::vector<vec3> vertices;

    GLenum draw_mode = GL_TRIANGLES;
};