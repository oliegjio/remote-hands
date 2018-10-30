#pragma once

#include "shape_group.h"

struct nested_group {
    nested_group() {};
    nested_group(std::initializer_list<shape_group*> arguments);

    void draw() const;

    std::vector<shape_group*> groups;

private:
    void draw_recursive(std::vector<shape_group*> groups) const;
};
