#pragma once

#include "nested_shape.h"

nested_shape *make_4_dof_hand() {
    vec3 limb_scale {1.0f, 3.0f, 1.0f};

    auto limb1 = shape::make_cube();
    limb1->scaling = limb_scale;
    limb1->rotation = vec4 {-90.0f, 0.0f, 0.0f, 1.0f};
    limb1->color = vec3 {1.0f, 0.0f, 0.0f};
    auto rotor1 = shape::make_cube();
    rotor1->translation = {2.0f, 0.0f, 0.0f};
    rotor1->scaling = {0.5, 2.0f, 0.5f};
    rotor1->color = vec3 {0.0f, 1.0f, 0.0f};

    auto limb2 = shape::make_cube();
    limb2->scaling = limb_scale;
    limb2->color = vec3 {0.0f, 1.0f, 0.0f};
    auto rotor2 = shape::make_cube();
    rotor2->translation = {0.0f, 2.0f, 0.0f};
    rotor2->scaling = {0.5f, 0.5f, 2.0f};
    rotor2->color = vec3 {0.0f, 0.0f, 1.0f};

    auto limb3 = shape::make_cube();
    limb3->scaling = limb_scale;
    limb3->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
    limb3->color = vec3 {0.0f, 0.0f, 1.0f};
    auto rotor3 = shape::make_cube();
    rotor3->translation = {-2.0f, 0.0f, 0.0f};
    rotor3->scaling = {0.5f, 0.5f, 2.0f};
    rotor3->color = vec3 {0.0f, 1.0f, 1.0f};

    auto limb4 = shape::make_cube();
    limb4->scaling = limb_scale;
    limb4->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
    limb4->color = vec3 {0.0f, 1.0f, 1.0f};
    auto rotor4 = shape::make_cube();
    rotor4->translation = {-2.0f, 0.0f, 0.0f};
    rotor4->scaling = {0.5f, 0.5f, 2.0f};
    rotor4->color = vec3 {1.0f, 1.0f, 0.0f};

    auto limb5 = shape::make_cube();
    limb5->scaling = limb_scale;
    limb5->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
    limb5->color = vec3 {1.0f, 1.0f, 0.0f};

    GLfloat hand_x = 0.0f;
    GLfloat hand_y = -10.0f;
    GLfloat hand_z = -65.0f;

    auto group1 = new shape_group {limb1, rotor1};
    group1->translation = {0.0f + hand_x, 0.0f + hand_y, 0.0f + hand_z};

    auto group2 = new shape_group {limb2, rotor2};
    group2->translation = {2.0f + hand_x, 6.0f + hand_y, 0.0f + hand_z};

    auto group3 = new shape_group {limb3, rotor3};
    group3->translation = {0.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};

    auto group4 = new shape_group {limb4, rotor4};
    group4->translation = {-8.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};

    auto group5 = new shape_group {limb5};
    group5->translation = {-16.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};

    std::vector<shape_group*> groups = {group1, group2, group3, group4, group5};
    auto hand = new nested_shape(groups);
    return hand;
}

void animate_4_dof_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float max_rotation = 45.0f;
    static float rotation_speed = 90.0f;

    hand->at(0)->rotation = {rotation, 0.0f, 1.0, 0.0f};
    hand->at(0)->translation = {2.0f, 0.0, 0.0f};
    hand->at(0)->group->shapes[1]->rotation = {rotation, 0.0f, 1.0f, 0.0f};

    hand->at(1)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(1)->translation = {0.0f, 2.0f, 0.0f};
    hand->at(1)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->at(2)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(2)->translation = {-2.0f, 0.0f, 0.0f};
    hand->at(2)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->at(3)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(3)->translation = {-2.0f, 0.0f, 0.0f};
    hand->at(3)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

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

nested_shape *make_one_plane_hand() {
    vec3 limb_scale {1.0f, 3.0f, 1.0f};
    vec3 rotor_scale {0.5f, 0.5f, 2.0f};

    auto limb1 = shape::make_cube();
    limb1->scaling = limb_scale;
    limb1->color = vec3 {0.0f, 1.0f, 0.0f};
    auto rotor1 = shape::make_cube();
    rotor1->translation = {0.0f, 2.0f, 0.0f};
    rotor1->scaling = rotor_scale;
    rotor1->color = vec3 {0.0f, 0.0f, 1.0f};

    auto limb2 = shape::make_cube();
    limb2->scaling = limb_scale;
    limb2->color = vec3 {0.0f, 0.0f, 1.0f};
    auto rotor2 = shape::make_cube();
    rotor2->translation = {0.0f, 2.0f, 0.0f};
    rotor2->scaling = rotor_scale;
    rotor2->color = vec3 {0.0f, 1.0f, 1.0f};

    auto limb3 = shape::make_cube();
    limb3->scaling = limb_scale;
    limb3->color = vec3 {0.0f, 1.0f, 1.0f};
    auto rotor3 = shape::make_cube();
    rotor3->translation = {0.0f, 2.0f, 0.0f};
    rotor3->scaling = rotor_scale;
    rotor3->color = vec3 {1.0f, 1.0f, 0.0f};

    auto limb4 = shape::make_cube();
    limb4->scaling = limb_scale;
    limb4->color = vec3 {1.0f, 1.0f, 0.0f};

    GLfloat hand_x = 0.0f;
    GLfloat hand_y = -10.0f;
    GLfloat hand_z = -65.0f;

    auto group1 = new shape_group {limb1, rotor1};
    group1->translation = {0.0f + hand_x, 0.0f + hand_y, 0.0f + hand_z};

    auto group2 = new shape_group {limb2, rotor2};
    group2->translation = {0.0f + hand_x, 8.0f + hand_y, 0.0f + hand_z};

    auto group3 = new shape_group {limb3, rotor3};
    group3->translation = {0.0f + hand_x, 16.0f + hand_y, 0.0f + hand_z};

    auto group4 = new shape_group {limb4};
    group4->translation = {0.0f + hand_x, 24.0f + hand_y, 0.0f + hand_z};

    std::vector<shape_group*> groups = {group1, group2, group3, group4};
    auto hand = new nested_shape(groups);
    return hand;
}

void animate_one_plane_hand(nested_shape *hand, const float &dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    static float max_rotation = 45.0f;
    static float rotation_speed = 90.0f;

    hand->at(0)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(0)->translation = {0.0f, 2.0f, 0.0f};
    hand->at(0)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->at(1)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(1)->translation = {0.0f, 2.0f, 0.0f};
    hand->at(1)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->at(2)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->at(2)->translation = {0.0f, 2.0f, 0.0f};
    hand->at(2)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

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
