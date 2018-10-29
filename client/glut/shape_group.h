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

    std::vector<shape*> shapes;
};

