#pragma once

#include "shape_group.h"

class nested_shape {
public:
    nested_shape() {};
    nested_shape(std::vector<shape_group> groups);

    void draw() const;

    shape_group group;
    nested_shape *child;
};
