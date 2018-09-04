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

}

void Canvas::resizeGL(int width, int height)
{

}

void Canvas::draw()
{
//    qglColor(Qt::red);
//        glBegin(GL_QUADS);
//            glNormal3f(0,0,-1);
//            glVertex3f(-1,-1,0);
//            glVertex3f(-1,1,0);
//            glVertex3f(1,1,0);
//            glVertex3f(1,-1,0);

//        glEnd();
//        glBegin(GL_TRIANGLES);
//            glNormal3f(0,-1,0.707);
//            glVertex3f(-1,-1,0);
//            glVertex3f(1,-1,0);
//            glVertex3f(0,0,1.2);
//        glEnd();
//        glBegin(GL_TRIANGLES);
//            glNormal3f(1,0, 0.707);
//            glVertex3f(1,-1,0);
//            glVertex3f(1,1,0);
//            glVertex3f(0,0,1.2);
//        glEnd();
//        glBegin(GL_TRIANGLES);
//            glNormal3f(0,1,0.707);
//            glVertex3f(1,1,0);
//            glVertex3f(-1,1,0);
//            glVertex3f(0,0,1.2);
//        glEnd();
//        glBegin(GL_TRIANGLES);
//            glNormal3f(-1,0,0.707);
//            glVertex3f(-1,1,0);
//            glVertex3f(-1,-1,0);
//            glVertex3f(0,0,1.2);
//        glEnd();
}
