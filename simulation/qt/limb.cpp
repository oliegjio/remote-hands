#include "limb.h"

Limb::Limb(GLfloat x, GLfloat y, GLfloat z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Limb::~Limb() {}

void Limb::draw()
{
    glLoadIdentity();
    glTranslatef(x - 5, y, z);
    glScalef(sX - 6, sY, sZ);
    glRotatef(angle, rX, rY, rZ);
    Shapes::drawWireCube(0, 1, 0, 0, 0, 1);

    glLoadIdentity();
    glTranslatef(x + 10, y, z);
    glScalef(sX - 6, sY, sZ);
    glRotatef(angle, rX, rY, rZ);
    Shapes::drawWireCube(1, 0, 0, 0, 1, 0);
}

void Limb::setRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    this->angle = angle;
    rX = x;
    rY = y;
    rZ = z;
}

void Limb::setTranslation(GLfloat x, GLfloat y, GLfloat z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Limb::setScale(GLfloat x, GLfloat y, GLfloat z)
{
    sX = x;
    sY = y;
    sZ = z;
}
