#pragma once

#include "arms.h"

void animate_planar_arm(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float min_rotation = -45.0f;
    static float max_rotation = 45.0f;
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

void animate_4dof_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    float min_rotation = -45.0f;
    float max_rotation = 45.0f;
    float rotation_speed = 90.0f;

    hand->shapes[0]->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->shapes[2]->rotation = {rotation, 0.0f, 1.0f, 0.0f};
    hand->shapes[4]->rotation = {rotation, 0.0f, 1.0f, 0.0f};
    hand->shapes[6]->rotation = {rotation, 0.0f, 1.0f, 0.0f};

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