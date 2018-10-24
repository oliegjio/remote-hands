#pragma once

#include <vector>

#include "shape.h"

class complex_shape {
public:
    complex_shape() {};

    void add_shape(const shape &new_shape);

    vec4 rotation { 0.0f, 0.0f, 0.0f, 0.0f };
    vec3 translation { 0.0f, 0.0f, 0.0f };
    vec3 scaling { 1.0f, 1.0f, 1.0f };

    void draw() const;

private:
    std::vector<shape> data;
};

