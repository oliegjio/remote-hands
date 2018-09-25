QT       += core gui widgets opengl

TARGET = remote-hands
TEMPLATE = app
# LIBS += -lglu32 -lopengl32

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    canvas.cpp

HEADERS += \
    mainwindow.h \
    canvas.h
