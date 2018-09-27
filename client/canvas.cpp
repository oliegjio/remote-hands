#include "canvas.h"

#include <GL/glu.h>

#include "shapes.h"

Canvas::Canvas() : QGLWidget(QGLFormat(QGL::SampleBuffers)) {}

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

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    static GLfloat ambient[4] = {0.6, 0.6, 0.6, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    static GLfloat diffuse[4] = {0.8, 0.8, 0.8, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    static GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};
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

static void mult(float *m, float *v)
{
    float tmp[4] = {v[0], v[1], v[2], v[3]};
    v[0] = m[0] * tmp[0] + m[1] * tmp[1] + m[2] * tmp[2] + m[3];
    v[1] = m[4] * tmp[0] + m[5] * tmp[1] + m[6] * tmp[2] + m[7];
    v[2] = m[8] * tmp[0] + m[9] * tmp[1] + m[10] * tmp[2] + m[11];
    v[3] = m[12] * tmp[0] + m[13] * tmp[1] + m[14] * tmp[2] + m[15];
}

void Canvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-5.0, 0.0, -10.0);
    glScalef(-5.0, 1.0, 1.0);
    glRotatef(rotation, 1, 1, 1);
    Shapes::drawCube();

    glLoadIdentity();
    glTranslatef(10.0, 0.0, -10.0);
    glScalef(-5.0, 1.0, 1.0);
    glRotatef(rotation, 1, 0, 0);

    Shapes::drawWireCube(1, 0, 0, 0, 1, 0);

    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    for (int i = 0; i < 4; i++)
    {
        qDebug() << m[i] << m[i + 1] << m[i + 2] << m[i + 3];
    }
    qDebug() << "================";

    glLoadIdentity();
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    float v1[] = {5, 5, -10};
    float v2[] = {0, 0, -10};
    mult(m, v1);
    mult(m, v2);
    glVertex3f(v1[0], v1[1], v1[2]);
    glVertex3f(v2[0], v2[1], v2[2]);
    glColor3f(1, 1, 1);
    glEnd();

    rotation += 1;

    update();
}

void Canvas::draw() { paintGL(); }
