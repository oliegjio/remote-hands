#pragma once

#include "nested_group.h"
#include "nested_shape.h"

nested_shape *make_one_plane_hand(GLfloat hand_x, GLfloat hand_y, GLfloat hand_z) {
    vec3 limb_scale {0.5f, 5.0f, 0.5f};
    vec3 rotor_scale {1.0f, 1.0f, 1.0f};

    auto limb1 = shape::make_cube();
    limb1->scaling = limb_scale;
    limb1->color = vec3 {0.0f, 1.0f, 0.0f};
    limb1->translation = {0.0f + hand_x, (limb_scale[1] * 0) + hand_y, 0.0f + hand_z};

    auto rotor1 = shape::make_cube();
    rotor1->translation = {0.0f + hand_x, -(limb_scale[1] * 1) + hand_y, 0.0f + hand_z};
    rotor1->scaling = rotor_scale;
    rotor1->color = vec3 {0.0f, 0.0f, 1.0f};

    auto limb2 = shape::make_cube();
    limb2->scaling = limb_scale;
    limb2->color = vec3 {0.0f, 0.0f, 1.0f};
    limb2->translation = {00.0f + hand_x, (limb_scale[1] * 2) + hand_y, 0.0f + hand_z};

    auto rotor2 = shape::make_cube();
    rotor2->translation = {0.0f + hand_x, (limb_scale[1] * 1) + hand_y, 0.0f + hand_z};
    rotor2->scaling = rotor_scale;
    rotor2->color = vec3 {0.0f, 1.0f, 1.0f};

    auto limb3 = shape::make_cube();
    limb3->scaling = limb_scale;
    limb3->color = vec3 {0.0f, 1.0f, 1.0f};
    limb3->translation = {0.0f + hand_x, (limb_scale[1] * 4) + hand_y, 0.0f + hand_z};

    auto rotor3 = shape::make_cube();
    rotor3->translation = {0.0f + hand_x, (limb_scale[1] * 3) + hand_y, 0.0f + hand_z};
    rotor3->scaling = rotor_scale;
    rotor3->color = vec3 {1.0f, 1.0f, 0.0f};

    auto hand = new nested_shape {rotor1, limb1, rotor2, limb2, rotor3, limb3};
    return hand;
}

void animate_one_plane_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float max_rotation = 45.0f;
    static float rotation_speed = 90.0f;

    hand->shapes[0]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[2]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    if (rotation < -max_rotation) {
        rotation_turn = false;
    } else if (rotation > max_rotation) {
        rotation_turn = true;
    }

    if (rotation_turn) {
        rotation -= rotation_speed * dt;
    } else {
        rotation += rotation_speed * dt;
    }
}
