#pragma once

#include "nested_shape.h"

class manipulator {
public:
    manipulator() {};
    void add_limb();

private:
    nested_shape *shape;
};

