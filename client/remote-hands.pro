QT       += core gui widgets opengl

TARGET = remote-hands
TEMPLATE = app
LIBS += -lglu32 -lopengl32 \
    -L"glut-3.7/lib" -lglut

INCLUDEPATH += glut-3.7/include

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    canvas.cpp \
    shapes.cpp

HEADERS += \
    mainwindow.h \
    canvas.h \
    shapes.h
