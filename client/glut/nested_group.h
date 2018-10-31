#pragma once

#include "shape_group.h"

struct nested_group {
    nested_group() {};
    nested_group(std::initializer_list<shape_group*> arguments);

    void draw() const;

    vec4 rotation {0.0f, 0.0f, 0.0f, 0.0f};
    vec3 translation {0.0f, 0.0f, 0.0f};
    vec3 scaling {1.0f, 1.0f, 1.0f};

    std::vector<shape_group*> groups;

private:
    void draw_recursive(std::vector<shape_group*> groups) const;
};
