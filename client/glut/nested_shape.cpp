#include "nested_shape.h"

void nested_shape::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
    glScalef(scaling[0], scaling[1], scaling[2]);

    draw_recursive(shapes);

    glPopMatrix();
}

nested_shape::nested_shape(std::initializer_list<shape *> arguments) {
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
        shapes.push_back(*it);
    }
}

void nested_shape::draw_recursive(std::vector<shape*> shapes) const {
    if (shapes.size() == 0) return;

    shapes[0]->draw();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(shapes[0]->translation[0], shapes[0]->translation[1], shapes[0]->translation[2]);
    glRotatef(shapes[0]->rotation[0], shapes[0]->rotation[1], shapes[0]->rotation[2], shapes[0]->rotation[3]);
    glTranslatef(-shapes[0]->translation[0], -shapes[0]->translation[1], -shapes[0]->translation[2]);

    shapes.erase(shapes.begin());
    draw_recursive(shapes);

    glPopMatrix();
}