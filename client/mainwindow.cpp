#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    canvas = new Canvas();
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout;
    timer = new QTimer;

    init();
}

MainWindow::~MainWindow() {}

void MainWindow::init()
{
    resize(800, 600);

    setCentralWidget(centralWidget);
    centralWidget->setLayout(mainLayout);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(canvas);

    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MainWindow::update()
{
    canvas->draw();
}
