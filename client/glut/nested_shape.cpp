#include "nested_shape.h"

void nested_shape::draw() {
    group->draw();

    if (child != nullptr) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glTranslatef(group->translation[0], group->translation[1], group->translation[2]);
        glRotatef(group->rotation[0], group->rotation[1], group->rotation[2], group->rotation[3]);
        glScalef(group->scaling[0], group->scaling[1], group->scaling[2]);

        child->draw();

        glPopMatrix();
    }
}

nested_shape::nested_shape(std::vector<shape_group*> groups) {
    if (groups.size() == 1) {
        group = groups.at(0);
        child = nullptr;
    } else {
        group = groups.at(0);
        groups.erase(groups.begin());
        child = new nested_shape(groups);
    }
}

