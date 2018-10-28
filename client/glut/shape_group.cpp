#include "shape_group.h"

#include "matrix.h"

void shape_group::draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
    glScalef(scaling[0], scaling[1], scaling[2]);

    float data[16];
    matrix4 transformation;
    glGetFloatv(GL_MODELVIEW_MATRIX, data);
    transformation = matrix4(data);
    transformation.data[3] = transformation.data[12];
    transformation.data[7] = transformation.data[13];
    transformation.data[11] = transformation.data[14];
    transformation.data[12] = 0.0f;
    transformation.data[13] = 0.0f;
    transformation.data[14] = 0.0f;
    basis_x = transformation * default_basis_x;
    basis_y = transformation * default_basis_y;
    basis_z = transformation * default_basis_z;
//    transformation.print();
//    std::cout << "--------------------------------" << std::endl;

    for (auto shape : shapes) {
        shape->draw();
    }

    glPopMatrix();
}

shape_group::shape_group(std::initializer_list<shape*> arguments) {
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
        shapes.push_back(*it);
    }
}
