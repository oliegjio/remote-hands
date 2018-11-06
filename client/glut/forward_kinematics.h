#pragma once

#include <cmath>

#include "mathematics.h"
#include "vector.h"

vector3 forward_kinematics_planar_arm(const float &angle1, const float &angle2, const float &angle3, const float &l1,
        const float &l2, const float &l3) {
    return vector3 {
        l1 * cosf(angle1) + l2 * cosf(angle1 + angle2) + l3 * cosf(angle1 + angle2 + angle3),
        l1 * sinf(angle1) + l2 * sinf(angle1 + angle2) + l3 * sinf(angle1 + angle2 + angle3),
        0.0f
    };
}

vector3 forward_kinematics_4dof_arm(const float &angle1, const float &angle2, const float &angle3, const float &angle4,
        const float &l1, const float &l2, const float &l3, const float &l4) {
    return vector3 {
            cosf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * cosf(angle1) * cosf(angle2 + angle3 + angle4),
            sinf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * sinf(angle1) * cosf(angle2 + angle3 + angle4),
            l1 + l2 * sinf(angle2) + l3 * sinf(angle2 + angle3) + l4 * sinf(angle2 + angle3 + angle4)
    };
}