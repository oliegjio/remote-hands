#pragma once

#include "nested_group.h"
#include "nested_shape.h"

nested_group *make_planar_arm() {
    vector3 limb_scale {5.0f, 0.5f, 0.5f};
    vector3 translation {5.0f, 0.0f, 0.0f};

    auto rotor1 = shape::make_cube();
    auto group1 = new shape_group {rotor1};
    rotor1->color = vector3 {0.0f, 0.0f, 1.0f};

    auto limb1 = shape::make_cube();
    auto group2 = new shape_group {limb1};
    group2->translation = translation;
    limb1->scaling = limb_scale;
    limb1->color = vector3 {0.0f, 1.0f, 0.0f};

    auto rotor2 = shape::make_cube();
    auto group3 = new shape_group {rotor2};
    group3->translation = translation;
    rotor2->color = vector3 {0.0f, 1.0f, 1.0f};

    auto limb2 = shape::make_cube();
    auto group4 = new shape_group {limb2};
    group4->translation = translation;
    limb2->scaling = limb_scale;
    limb2->color = vector3 {0.0f, 0.0f, 1.0f};

    auto rotor3 = shape::make_cube();
    auto group5 = new shape_group {rotor3};
    group5->translation = translation;
    rotor3->color = vector3 {1.0f, 1.0f, 0.0f};

    auto limb3 = shape::make_cube();
    auto group6 = new shape_group {limb3};
    group6->translation = translation;
    limb3->scaling = limb_scale;
    limb3->color = vector3 {0.0f, 1.0f, 1.0f};

    auto arm = new nested_group {group1, group2, group3, group4, group5, group6};
    return arm;
}

nested_shape *make_4dof_arm() {
    auto rotor1 = shape::make_cube();
    rotor1->color = vector3 {0.0f, 0.0f, 1.0f};

    auto limb1 = shape::make_cube();
    limb1->translation = {0.0f, 0.0f, 5.0f};
    limb1->scaling = {0.5f, 0.5f, 5.0f};
    limb1->color = vector3 {0.0f, 1.0f, 0.0f};

    auto rotor2 = shape::make_cube();
    rotor2->translation = {0.0f, 0.0f, 10.0f};
    rotor2->color = vector3 {0.0f, 1.0f, 1.0f};

    auto limb2 = shape::make_cube();
    limb2->translation = {5.0f, 0.0f, 10.0f};
    limb2->scaling = {5.0f, 0.5f, 0.5f};
    limb2->color = vector3 {0.0f, 0.0f, 1.0f};

    auto rotor3 = shape::make_cube();
    rotor3->translation = {10.0f, 0.0f, 10.0f};
    rotor3->color = vector3 {1.0f, 1.0f, 0.0f};

    auto limb3 = shape::make_cube();
    limb3->translation = {15.0f, 0.0f, 10.0f};
    limb3->scaling = {5.0f, 0.5f, 0.5f};
    limb3->color = vector3 {0.0f, 1.0f, 1.0f};

    auto rotor4 = shape::make_cube();
    rotor4->translation = {20.0f, 0.0f, 10.0f};
    rotor4->color = vector3 {1.0f, 0.0f, 0.5f};

    auto limb4 = shape::make_cube();
    limb4->translation = {25.0f, 0.0f, 10.0f};
    limb4->scaling = {5.0f, 0.5f, 0.5f};
    limb4->color = vector3 {1.0f, 1.0f, 0.0f};

    auto hand = new nested_shape {rotor1, limb1, rotor2, limb2, rotor3, limb3, rotor4, limb4};
    return hand;
}
