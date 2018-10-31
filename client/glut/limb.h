#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/freeglut.h>

#include "shape_group.h"

struct limb {
    limb(GLfloat length) {};

    shape_group *shape;
};

