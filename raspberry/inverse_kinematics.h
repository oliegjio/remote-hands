#pragma once

#include <cmath>

#include "mathematics.h"
#include "vector.h"

vector3 inverse_kinematics_planar_arm(const float &x, const float &y, const float &l1, const float &l2,
        const float &l3) {
    float phi = atan2f(y, x);
    float xw = x - l3 * cosf(phi);
    float yw = y - l3 * sinf(phi);
    float r = powf(xw, 2.0f) + powf(yw, 2.0f);
    float beta = acosf((powf(l1, 2.0f) + powf(l2, 2.0f) - r) / (2.0f * l1 * l2));
    float gamma = acosf((r  + powf(l1, 2.0f) - powf(l2, 2.0f)) / (2.0f * sqrtf(r) * l1));
    float alpha = atan2f(yw, xw);

    float r1 = alpha - gamma;
    float r2 = PI - beta;
    float r3 = phi - r1 - r2;
    return vector3 {r1, r2, r3};
}

vector4 inverse_kinematics_4dof_arm(const float &x, const float &y, const float &z, const float &angle1,
        const float &angle3, const float &l1, const float &l2, const float &l3, const float &l4) {
//    float len = sqrtf(powf(x, 2.0f) + powf(y, 2.0f));
//    float phi = atan2f(z, len);
//
//    float a = l3 * sinf(angle3);
//    float b = l2 + l3 * cosf(angle3);
//    float c = z - l1 - l4 * sinf(phi);
//    float r = sqrtf(powf(a, 2.0f) + powf(b, 2.0f));
//    float A = x - l4 * cosf(angle1) * cosf(phi);
//    float B = y - l4 * sinf(angle1) * cosf(phi);
//    float C = z - l1 - l4 * sinf(phi);
//
//    float r1 = atan2f(y, x);
//    float r2 = atan2f(c, sqrtf(powf(r, 2.0f) - powf(c, 2.0f))) - atan2f(a, b);
//    float r3 = acosf((powf(A, 2.0f) + powf(B, 2.0f) + powf(C, 2.0f) - powf(l2, 2.0f) - powf(l3, 2.0f)) / (2.0f * l2 * l3));
//    float r4 = phi - r2 - r3;

////////////////////

//    float xn = sqrtf(powf(x, 2.0f) + powf(y, 2.0f));
//    float yn =

    float phi = atan2f(z, x);
    float xw = x - l3 * cosf(phi);
    float yw = y - l3 * sinf(phi);
    float r = powf(xw, 2.0f) + powf(yw, 2.0f);
    float beta = acosf((powf(l1, 2.0f) + powf(l2, 2.0f) - r) / (2.0f * l1 * l2));
    float gamma = acosf((r  + powf(l1, 2.0f) - powf(l2, 2.0f)) / (2.0f * sqrtf(r) * l1));
    float alpha = atan2f(yw, xw);

    float r1 = 0.0f;
    float r2 = alpha - gamma;
    float r3 = PI - beta;
    float r4 = phi - r2 - r3;

    return vector4 {r1, r2, r3, r4};
}