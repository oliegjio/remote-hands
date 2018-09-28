QT       += core gui widgets opengl

TARGET = remote-hands
TEMPLATE = app
win32:LIBS += -lglu32 -lopengl32

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    canvas.cpp \
    shapes.cpp \
    limb.cpp

HEADERS += \
    mainwindow.h \
    canvas.h \
    shapes.h \
    limb.h
