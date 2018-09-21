#ifndef CANVAS_H
#define CANVAS_H

#include <QtOpenGL>
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    ~Canvas();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void draw();
};

#endif // CANVAS_H
