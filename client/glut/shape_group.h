#pragma once

#include <vector>

#include "shape.h"

struct shape_group {
    shape_group() {};
    shape_group(std::initializer_list<shape*> arguments);

    void draw();

    vec4 rotation { 0.0f, 0.0f, 0.0f, 0.0f };
    vec3 translation { 0.0f, 0.0f, 0.0f };
    vec3 scaling { 1.0f, 1.0f, 1.0f };

    vec4 default_basis_x {1.0f, 0.0f, 0.0f, 1.0f};
    vec4 default_basis_y {0.0f, 1.0f, 0.0f, 1.0f};
    vec4 default_basis_z {0.0f, 0.0f, 1.0f, 1.0f};

    vec4 basis_x = default_basis_x;
    vec4 basis_y = default_basis_y;
    vec4 basis_z = default_basis_z;

    std::vector<shape*> shapes;
};

