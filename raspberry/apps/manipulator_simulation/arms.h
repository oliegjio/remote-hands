#pragma once

#include "nested_group.h"
#include "nested_shape.h"

nested_group *make_planar_arm() {
    const float limb_length = 10.0f;
    const float limb_thickness = 0.5;

    const vector<3> limb_scale {limb_length / 2.0f, limb_thickness, limb_thickness};
    const vector<3> translation {limb_length / 2.0f, 0.0f, 0.0f};

    auto rotor1 = shape::make_cube();
    auto group1 = new shape_group {rotor1};
    rotor1->color = vector<3> {0.0f, 0.0f, 1.0f};

    auto limb1 = shape::make_cube();
    auto group2 = new shape_group {limb1};
    group2->translation = translation;
    limb1->scaling = limb_scale;
    limb1->color = vector<3> {0.0f, 1.0f, 0.0f};

    auto rotor2 = shape::make_cube();
    auto group3 = new shape_group {rotor2};
    group3->translation = translation;
    rotor2->color = vector<3> {0.0f, 1.0f, 1.0f};

    auto limb2 = shape::make_cube();
    auto group4 = new shape_group {limb2};
    group4->translation = translation;
    limb2->scaling = limb_scale;
    limb2->color = vector<3> {0.0f, 0.0f, 1.0f};

    auto rotor3 = shape::make_cube();
    auto group5 = new shape_group {rotor3};
    group5->translation = translation;
    rotor3->color = vector<3> {1.0f, 1.0f, 0.0f};

    auto limb3 = shape::make_cube();
    auto group6 = new shape_group {limb3};
    group6->translation = translation;
    limb3->scaling = limb_scale;
    limb3->color = vector<3> {0.0f, 1.0f, 1.0f};

    auto arm = new nested_group {group1, group2, group3, group4, group5, group6};
    return arm;
}

nested_group *make_4dof_arm() {
    const float limb_length = 10.0f;
    const float limb_thickness = 0.5;

    const vector<3> vertical_limb_scale {limb_thickness, limb_thickness, limb_length / 2.0f};
    const vector<3> horizontal_limb_scale {limb_length / 2.0f, limb_thickness, limb_thickness};
    const vector<3> vertical_translation {0.0f, 0.0f, limb_length / 2.0f};
    const vector<3> horizontal_translation {limb_length / 2.0f, 0.0f, 0.0f};

    auto rotor1 = shape::make_cube();
    rotor1->color = vector<3> {0.0f, 0.0f, 1.0f};
    auto group1 = new shape_group {rotor1};

    auto limb1 = shape::make_cube();
    limb1->scaling = vertical_limb_scale;
    limb1->color = vector<3> {0.0f, 1.0f, 0.0f};
    auto group2 = new shape_group {limb1};
    group2->translation = vertical_translation;

    auto rotor2 = shape::make_cube();
    rotor2->color = vector<3> {0.0f, 1.0f, 1.0f};
    auto group3 = new shape_group {rotor2};
    group3->translation = vertical_translation;

    auto limb2 = shape::make_cube();
    limb2->scaling = horizontal_limb_scale;
    limb2->color = vector<3> {0.0f, 0.0f, 1.0f};
    auto group4 = new shape_group {limb2};
    group4->translation = horizontal_translation;

    auto rotor3 = shape::make_cube();
    rotor3->color = vector<3> {1.0f, 1.0f, 0.0f};
    auto group5 = new shape_group {rotor3};
    group5->translation = horizontal_translation;

    auto limb3 = shape::make_cube();
    limb3->scaling = horizontal_limb_scale;
    limb3->color = vector<3> {0.0f, 1.0f, 1.0f};
    auto group6 = new shape_group {limb3};
    group6->translation = horizontal_translation;

    auto rotor4 = shape::make_cube();
    rotor4->color = vector<3> {1.0f, 0.0f, 0.5f};
    auto group7 = new shape_group {rotor4};
    group7->translation = horizontal_translation;

    auto limb4 = shape::make_cube();
    limb4->scaling = horizontal_limb_scale;
    limb4->color = vector<3> {1.0f, 1.0f, 0.0f};
    auto group8 = new shape_group {limb4};
    group8->translation = horizontal_translation;

    auto hand = new nested_group {group1, group2, group3, group4, group5, group6, group7, group8};
    return hand;
}
