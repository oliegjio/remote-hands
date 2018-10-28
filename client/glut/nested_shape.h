#pragma once

#include "shape_group.h"

struct nested_shape {
    nested_shape() {};
    nested_shape(std::vector<shape_group*> groups);

    void draw();

    shape_group *group;
    nested_shape *child;
};
