#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <vector>

#include "vector.h"

struct shape {
    shape() = default;
	static shape *make_cube();

	void draw() const;

	vector<4> rotation {0.0f, 0.0f, 0.0f, 0.0f};
	vector<3> translation {0.0f, 0.0f, 0.0f};
    vector<3> scaling {1.0f, 1.0f, 1.0f};

    vector<3> color {0.0f, 0.0f, 0.0f};

    std::vector<vector<3>> vertices;

    GLenum draw_mode = GL_TRIANGLES;
};