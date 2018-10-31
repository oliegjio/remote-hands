#pragma once

#include "nested_group.h"

class manipulator {
public:
    manipulator() {};
    void add_limb();

private:
    nested_group *shape;
};

