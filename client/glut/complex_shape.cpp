#include "complex_shape.h"

void complex_shape::add_shape(const shape &new_shape) {
    shapes.push_back(new_shape);
}

void complex_shape::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
    glScalef(scaling[0], scaling[1], scaling[2]);

    for (auto shape : shapes) {
        shape.draw();
    }

    glPopMatrix();
}