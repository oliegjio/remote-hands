#include "nested_group.h"

void nested_group::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
    glScalef(scaling[0], scaling[1], scaling[2]);

    draw_recursive(groups);

    glPopMatrix();
}

nested_group::nested_group(std::initializer_list<shape_group *> arguments) {
    for (auto it : arguments) {
        groups.push_back(it);
    }
}

void nested_group::draw_recursive(std::vector<shape_group*> groups) const {
    if (groups.empty()) return;

    groups[0]->draw();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(groups[0]->translation[0], groups[0]->translation[1], groups[0]->translation[2]);
    glRotatef(groups[0]->rotation[0], groups[0]->rotation[1], groups[0]->rotation[2], groups[0]->rotation[3]);
    glScalef(groups[0]->scaling[0], groups[0]->scaling[1], groups[0]->scaling[2]);

    groups.erase(groups.begin());
    draw_recursive(groups);

    glPopMatrix();
}




