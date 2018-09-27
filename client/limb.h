#ifndef LIMB_H
#define LIMB_H

#include <GL/gl.h>
#include <QObject>

#include "shapes.h"

class Limb : public QObject
{
    Q_OBJECT

public:
    Limb(GLfloat x, GLfloat y, GLfloat z);
    ~Limb();

    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;

    GLfloat rX = 0;
    GLfloat rY = 0;
    GLfloat rZ = 0;
    GLfloat angle = 0;

    GLfloat sX = 1;
    GLfloat sY = 1;
    GLfloat sZ = 1;

    void draw();

    void setRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void setTranslation(GLfloat x, GLfloat y, GLfloat z);
    void setScale(GLfloat x, GLfloat y, GLfloat z);
};

#endif // LIMB_H
