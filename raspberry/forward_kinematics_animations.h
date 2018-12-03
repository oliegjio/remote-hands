#pragma once

#include "mathematics.h"
#include "nested_shape.h"
#include "forward_kinematics.h"

void animate_forward_kinematics_planar_arm(nested_shape *arm, shape *follower, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    float min_rotation = -45.0f;
    float max_rotation = 45.0f;
    float rotation_speed = 90.0f;
    float l = 10.0f;

    arm->shapes[0]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    arm->shapes[2]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    arm->shapes[4]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    if (rotation < min_rotation) {
        rotation_turn = false;
    } else if (rotation > max_rotation) {
        rotation_turn = true;
    }

    if (rotation_turn) {
        rotation -= rotation_speed * dt;
    } else {
        rotation += rotation_speed * dt;
    }

    float rad_rotation = rotation * (PI / 180.0f);
    vector3 position = forward_kinematics_planar_arm(rad_rotation, rad_rotation, rad_rotation, l, l, l);

    follower->translation = position;
}