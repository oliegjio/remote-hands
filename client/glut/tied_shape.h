#pragma once

#include "shape_group.h"

class tied_shape {
public:
    tied_shape() {}

    void draw() const;

    vec4 rotation { 0.0f, 0.0f, 0.0f, 0.0f };
    vec3 translation { 0.0f, 0.0f, 0.0f };
    vec3 scaling { 1.0f, 1.0f, 1.0f };

    std::vector<shape_group> groups;
};
