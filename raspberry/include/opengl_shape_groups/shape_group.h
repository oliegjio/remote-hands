#pragma once

#include <vector>

#include "shape.h"

struct shape_group {
    shape_group() = default;
    shape_group(std::initializer_list<shape*> arguments);

    void draw() const;

    vector<4> rotation {0.0f, 0.0f, 0.0f, 0.0f};
    vector<3> translation {0.0f, 0.0f, 0.0f};
    vector<3> scaling {1.0f, 1.0f, 1.0f};

    std::vector<shape*> shapes;
};

