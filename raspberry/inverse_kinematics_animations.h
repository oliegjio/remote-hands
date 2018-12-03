#pragma once

#include "inverse_kinematics.h"
#include "nested_group.h"
#include "nested_shape.h"

void animate_inverse_kinematics_planar_arm(nested_group *arm, shape *follower, const vector3 &position) {
    float l = 10.0f;

    vector3 angles = inverse_kinematics_planar_arm(position[0], position[1], l, l, l);

    angles *= (180.0f / PI);
    arm->groups[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    arm->groups[2]->rotation = {angles[1], 0.0f, 0.0f, 1.0f};
    arm->groups[4]->rotation = {angles[2], 0.0f, 0.0f, 1.0f};

    follower->translation = {position[0], position[1], position[2]};
}

void animate_inverse_kinematics_4dof_arm(nested_shape *arm, shape *follower, const vector3 &position) {
    // TODO: Refactor for `nested_group`.

    static float angle1 = 0.0f;
    static float angle2 = 0.0f;
    float l = 10.0f;

    vector4 angles = inverse_kinematics_4dof_arm(position[0], position[1], position[2], angle1, angle2, l, l, l, l);

    angles *= (180.0f / PI);
    arm->shapes[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    arm->shapes[2]->rotation = {angles[1], 0.0f, 1.0f, 0.0f};
    arm->shapes[4]->rotation = {angles[2], 0.0f, 1.0f, 0.0f};
    arm->shapes[6]->rotation = {angles[3], 0.0f, 1.0f, 0.0f};

    follower->translation = {position[0], position[1], position[2]};
}