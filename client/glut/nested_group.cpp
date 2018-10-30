#include "nested_group.h"

void nested_group::draw() const {
    draw_recursive(groups);
}

nested_group::nested_group(std::initializer_list<shape_group *> arguments) {
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
        groups.push_back(*it);
    }
}

void nested_group::draw_recursive(std::vector<shape_group*> groups) const {
    if (groups.size() == 0) return;

    groups[0]->draw();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(groups[0]->translation[0], groups[0]->translation[1], groups[0]->translation[2]);
    glRotatef(groups[0]->rotation[0], groups[0]->rotation[1], groups[0]->rotation[2], groups[0]->rotation[3]);
    glTranslatef(-groups[0]->translation[0], -groups[0]->translation[1], -groups[0]->translation[2]);
    glScalef(groups[0]->scaling[0], groups[0]->scaling[1], groups[0]->scaling[2]);

    groups.erase(groups.begin());
    draw_recursive(groups);

    glPopMatrix();
}




