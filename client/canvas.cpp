#include "canvas.h"

#include <GL/glu.h>

Canvas::Canvas() : QGLWidget(QGLFormat(QGL::SampleBuffers))
{
    leftLimb = new Limb(0, 0, -10);
}

Canvas::~Canvas() {}

void Canvas::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    static GLfloat ambient[4] = {0.6f, 0.6f, 0.6f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    static GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    static GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void Canvas::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -2, 2, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void Canvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    leftLimb->setRotation(rotation, 0, 1, 0);
    leftLimb->draw();

    rotation += 1;

    update();
}

void Canvas::draw() { paintGL(); }
