#pragma once

#include <vector>

#include "shape.h"

struct shape_group {
    shape_group() {};
    shape_group(std::initializer_list<shape*> arguments);

    void draw() const;

    vector4 rotation {0.0f, 0.0f, 0.0f, 0.0f};
    vector3 translation {0.0f, 0.0f, 0.0f};
    vector3 scaling {1.0f, 1.0f, 1.0f};

    std::vector<shape*> shapes;
};

