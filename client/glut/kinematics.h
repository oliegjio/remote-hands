#pragma once

#include <cmath>

#include "vec.h"
#include "nested_shape.h"

#define PI 3.14159265f

vec2 forward_kinematics_planar_hand(float angle1, float angle2, float angle3, float l1, float l2, float l3) {
    return vec2 {
        l1 * cosf(angle1) + l2 * cosf(angle1 + angle2) + l3 * cosf(angle1 + angle2 + angle3),
        l1 * sinf(angle1) + l2 * sinf(angle1 + angle2) + l3 * sinf(angle1 + angle2 + angle3)
    };
}

void animate_forward_kinematics_planar_hand(nested_shape *hand, shape *follower, float dt) {
    static float rotation = 0.0f;
    static bool rotation_turn = true;
    float min_rotation = -45.0f;
    float max_rotation = 45.0f;
    float rotation_speed = 90.0f;
    float l = 10.0f;

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

    float rad_rotation = rotation * (PI / 180.0f);
    vec2 position = forward_kinematics_planar_hand(rad_rotation, rad_rotation, rad_rotation, l, l, l);

    follower->translation = {position[0], position[1], 0.0f};

}

vec3 inverse_kinematics_planar_hand(float x, float y, float angle1, float angle2, float angle3, float phi,
        float l1, float l2, float l3) {
//    float xn = x - l3 * cosf(phi);
//    float yn = y - l3 * sinf(phi);
//    float cos = (powf(xn, 2.0f) + powf(yn, 2.0f) - powf(l1, 2.0f) - powf(l2, 2.0f)) / (2 * l1 * l2);
//    float sin = sqrtf(fabsf(1 - powf(cos, 2.0f)));
//    float k1 = l1 + l2 * cos;
//    float k2 = l2 * sin;
//
//    float r1 = atan2f(k1 * yn - k2 * xn, k1 * xn - k2 * yn);
//    float r2 = atan2f(sin, cos);
//    float r3 = phi - r1 - r2;
//    return vec3 {r1, r2, r3};

    float xw = x - l3 * cosf(phi);
    float yw = y - l3 * sinf(phi);
    float r = powf(xw, 2.0f) + powf(yw, 2.0f);
    float beta = 1.0f / cosf((powf(l1, 2.0f) + powf(l2, 2.0f) - r) / (2.0f * l1 * l2));
    float gamma = 1.0f / cosf((r  + powf(l1, 2.0f) - powf(l2, 2.0f)) / (2.0f * sqrtf(r) * l1));
    float alpha = 1.0f / tanf(yw / xw);

    float r1 = alpha - gamma;
    float r2 = PI - beta;
    float r3 = phi - r1 - r2;
    return vec3 {r1, r2, r3};
}

void animate_inverse_kinematics_planar_hand(nested_shape *hand, shape *follower, vec2 position) {
    static float angle1 = 0.0f;
    static float angle2 = 0.0f;
    static float angle3 = 0.0f;
    static float phi = 0.0f + (PI / 4.0f);
    float l = 10.0f;

    vec3 angles = inverse_kinematics_planar_hand(position[0], position[1], angle1, angle2, angle3, phi, l, l, l);

//    angle1 = angles[0];
//    angle2 = angles[1];
//    angle3 = angles[2];
//    phi = angles[0] + angles[1] + angles[2];

    angles *= (180.0f / PI);
    hand->shapes[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    hand->shapes[2]->rotation = {angles[1], 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {angles[2], 0.0f, 0.0f, 1.0f};

    follower->translation = {position[0], position[1], 0.0f};
}

vec3 forward_kinematics_4dof_hand(float angle1, float angle2, float angle3, float angle4, float l1, float l2,
                                  float l3, float l4) {
    return vec3 {
        cosf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * cosf(angle1) * cosf(angle2 + angle3 + angle4),
        sinf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * sinf(angle1) * cosf(angle2 + angle3 + angle4),
        l1 + l2 * sinf(angle2) + l3 * sinf(angle2 + angle3) + l4 * sinf(angle2 + angle3 + angle4)
    };
}

vec4 inverse_kinematics_4dof_hand(float x, float y, float z, float angle1, float angle2, float angle3, float angle4,
        float phi, float l1, float l2, float l3, float l4) {
    float a = l3 * sinf(angle3);
    float b = l2 + l3 * cosf(angle3);
    float c = z - l1 - l4 * sinf(phi);
    float r = sqrtf(powf(a, 2.0f) + powf(b, 2.0f));
    float A = x - l4 * cosf(angle1) * cosf(phi);
    float B = y - l4 * sinf(angle1) * cosf(phi);
    float C = z - l1 - l4 * sinf(phi);

    float r1 = atanf(x / y);
    float r2 = atan2f(c, fabsf(sqrtf(powf(r, 2.0f) - powf(c, 2.0f)))) - atan2f(a, b);
    float r3 = acosf((powf(A, 2.0f) + powf(B, 2.0f) + powf(C, 2.0f) - powf(l2, 2.0f) - powf(l3, 2.0f)) / (2 * l2 * l3));
    float r4 = phi - r2 - r3;

    return vec4 {r1, r2, r3, r4};
}