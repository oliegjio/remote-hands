#pragma once

#include <vector>

#include "shape.h"

struct nested_shape {
    nested_shape() = default;
    nested_shape(std::initializer_list<shape*> arguments);

    void draw() const;

    vector<4> rotation {0.0f, 0.0f, 0.0f, 0.0f};
    vector<3> translation {0.0f, 0.0f, 0.0f};
    vector<3> scaling {1.0f, 1.0f, 1.0f};

    std::vector<shape*> shapes;

private:
    void draw_recursive(std::vector<shape*> shapes) const;
};

