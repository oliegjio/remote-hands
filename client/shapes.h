#ifndef SHAPES_H
#define SHAPES_H

#include <GL/glu.h>

class Shapes
{
public:
    static void drawCube();
    static void drawCube(GLfloat r, GLfloat g, GLfloat b);
    static void drawWireCube(GLfloat r1, GLfloat g1, GLfloat b1, GLfloat r2, GLfloat g2, GLfloat b2, GLfloat width = 2);
};

#endif // SHAPES_H
