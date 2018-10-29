#include "nested_shape.h"

void nested_shape::draw() {
    group->draw();

    if (child != nullptr) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glTranslatef(group->translation[0] + translation[0], group->translation[1] + translation[1], group->translation[2] + translation[2]);
        glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
        glTranslatef(-group->translation[0] - translation[0], -group->translation[1] - translation[1], -group->translation[2] - translation[2]);
        glScalef(scaling[0], scaling[1], scaling[2]);

        child->draw();

        glPopMatrix();
    }
}

nested_shape::nested_shape(std::vector<shape_group*> groups) {
    group = groups.at(0);
    if (groups.size() == 1) {
        child = nullptr;
    } else {
        groups.erase(groups.begin());
        child = new nested_shape(groups);
    }
}



