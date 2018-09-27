#include "shapes.h"

void Shapes::drawCube()
{
    glBegin(GL_QUADS);
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

        glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)

        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

        glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)

        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
    glEnd();
}

void Shapes::drawCube(GLfloat r, GLfloat g, GLfloat b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

        glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)

        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

        glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)

        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
    glEnd();
}

void Shapes::drawWireCube(GLfloat r1, GLfloat g1, GLfloat b1, GLfloat r2, GLfloat g2, GLfloat b2, GLfloat width)
{
    glColor3f(r1, g1, b1);
    glBegin(GL_QUADS);
        glColor3f(r1, g1, b1);
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

        glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)

        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

        glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)

        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
    glEnd();

    glColor3f(r2, g2, b2);
    glLineWidth(width);
    glBegin(GL_LINES);
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

        glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)

        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

        glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)

        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
    glEnd();
}
