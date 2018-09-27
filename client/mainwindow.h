#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    Canvas *canvas;

    QTimer *timer;

    void init();

public slots:
    void update();
};

#endif // MAINWINDOW_H
