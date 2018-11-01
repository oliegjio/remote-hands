#pragma once

#include "nested_group.h"
#include "nested_shape.h"

nested_shape *make_planar_hand() {
    vec3 limb_scale {5.0f, 0.5f, 0.5f};

    auto rotor1 = shape::make_cube();
    rotor1->color = vec3 {0.0f, 0.0f, 1.0f};

    auto limb1 = shape::make_cube();
    limb1->translation = {limb_scale[0] * 1.0f, 0.0f, 0.0f};
    limb1->scaling = limb_scale;
    limb1->color = vec3 {0.0f, 1.0f, 0.0f};

    auto rotor2 = shape::make_cube();
    rotor2->translation = {limb_scale[0] * 2.0f, 0.0f, 0.0f};
    rotor2->color = vec3 {0.0f, 1.0f, 1.0f};

    auto limb2 = shape::make_cube();
    limb2->translation = {limb_scale[0] * 3.0f, 0.0f, 0.0f};
    limb2->scaling = limb_scale;
    limb2->color = vec3 {0.0f, 0.0f, 1.0f};

    auto rotor3 = shape::make_cube();
    rotor3->translation = {limb_scale[0] * 4.0f, 0.0f, 0.0f};
    rotor3->color = vec3 {1.0f, 1.0f, 0.0f};

    auto limb3 = shape::make_cube();
    limb3->translation = {limb_scale[0] * 5.0f, 0.0f, 0.0f};
    limb3->scaling = limb_scale;
    limb3->color = vec3 {0.0f, 1.0f, 1.0f};

    auto hand = new nested_shape {rotor1, limb1, rotor2, limb2, rotor3, limb3};
    return hand;
}

void animate_planar_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float min_rotation = -45.0f;
    static float max_rotation = 0.0f;
    static float rotation_speed = 90.0f;

    hand->shapes[0]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[2]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

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
}

nested_shape *make_4dof_hand() {
    vec3 limb_scale {0.5f, 5.0f, 0.5f};

    auto rotor1 = shape::make_cube();
    rotor1->color = vec3 {0.0f, limb_scale[1] * 0.0f, 1.0f};

    auto limb1 = shape::make_cube();
    limb1->translation = {0.0f, 5.0f, 0.0f};
    limb1->scaling = limb_scale;
    limb1->color = vec3 {0.0f, 1.0f, 0.0f};

    auto rotor2 = shape::make_cube();
    rotor2->translation = {0.0f, 10.0f, 0.0f};
    rotor2->color = vec3 {0.0f, 1.0f, 1.0f};

    auto limb2 = shape::make_cube();
    limb2->translation = {5.0f, 10.0f, 0.0f};
    limb2->scaling = {5.0f, 0.5f, 0.5f};
    limb2->color = vec3 {0.0f, 0.0f, 1.0f};

    auto rotor3 = shape::make_cube();
    rotor3->translation = {10.0f, 10.0f, 0.0f};
    rotor3->color = vec3 {1.0f, 1.0f, 0.0f};

    auto limb3 = shape::make_cube();
    limb3->translation = {15.0f, 10.0f, 0.0f};
    limb3->scaling = {5.0f, 0.5f, 0.5f};
    limb3->color = vec3 {0.0f, 1.0f, 1.0f};

    auto rotor4 = shape::make_cube();
    rotor4->translation = {20.0f, 10.0f, 0.0f};
    rotor4->color = vec3 {1.0f, 0.0f, 0.5f};

    auto limb4 = shape::make_cube();
    limb4->translation = {25.0f, 10.0f, 0.0f};
    limb4->scaling = {5.0f, 0.5f, 0.5f};
    limb4->color = vec3 {1.0f, 1.0f, 0.0f};

    auto hand = new nested_shape {rotor1, limb1, rotor2, limb2, rotor3, limb3, rotor4, limb4};
    return hand;
}

void animate_4dof_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float min_rotation = -45.0f;
    static float max_rotation = 0.0f;
    static float rotation_speed = 90.0f;

    hand->shapes[0]->rotation = {rotation, 0.0f, 1.0f, 0.0f};
    hand->shapes[2]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[6]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

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
}