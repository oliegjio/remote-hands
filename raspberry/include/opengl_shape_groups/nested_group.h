#pragma once

#include "shape_group.h"

struct nested_group {
    nested_group() = default;
    nested_group(std::initializer_list<shape_group*> arguments);

    void draw() const;

    vector<4> rotation {0.0f, 0.0f, 0.0f, 0.0f};
    vector<3> translation {0.0f, 0.0f, 0.0f};
    vector<3> scaling {1.0f, 1.0f, 1.0f};

    std::vector<shape_group*> groups;

private:
    void draw_recursive(std::vector<shape_group*> groups) const;
};
