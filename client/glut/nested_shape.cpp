#include "nested_shape.h"

void nested_shape::draw() {
    group->draw();

    if (child != nullptr) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

//        glTranslatef(group->translation[0], group->translation[1], group->translation[2]);
//        glRotatef(group->rotation[0], group->rotation[1], group->rotation[2], group->rotation[3]);
//        glTranslatef(-group->translation[0], -group->translation[1], -group->translation[2]);
//        glScalef(group->scaling[0], group->scaling[1], group->scaling[2]);

//        glTranslatef(translation[0], translation[1], translation[2]);
        glTranslatef(group->translation[0] + translation[0], group->translation[1] + translation[1], group->translation[2] + translation[2]);
        glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
//        glTranslatef(translation[0], translation[1], translation[2]);
        glTranslatef(-group->translation[0] - translation[0], -group->translation[1] - translation[1], -group->translation[2] - translation[2]);
        glScalef(scaling[0], scaling[1], scaling[2]);

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


