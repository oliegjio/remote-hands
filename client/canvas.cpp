#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QGLWidget(parent)
{

}

Canvas::~Canvas() {}

void Canvas::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void Canvas::paintGL()
{
    glBegin(GL_POLYGON);
            glColor4f(0,1,0, 0.25);
            glVertex2f(200, 300);
            glVertex2f(300, 300);
            glVertex2f(300, 400);
            glVertex2f(200, 400);
    glEnd();
    swapBuffers();
}

void Canvas::resizeGL(int width, int height)
{

}

void Canvas::draw()
{

}
