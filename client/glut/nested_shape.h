#pragma once

#include "shape_group.h"

struct nested_shape {
    nested_shape() {};
    nested_shape(std::vector<shape_group*> groups);

    void draw();

    vec4 rotation { 0.0f, 0.0f, 0.0f, 0.0f };
    vec3 translation { 0.0f, 0.0f, 0.0f };
    vec3 scaling { 1.0f, 1.0f, 1.0f };

    shape_group *group;
    nested_shape *child;
};
