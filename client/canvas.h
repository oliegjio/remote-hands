#ifndef CANVAS_H
#define CANVAS_H

#include <QtOpenGL>
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT

public:
    Canvas();
    ~Canvas();

    void draw();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    GLfloat rotation = 0;
};

#endif // CANVAS_H
