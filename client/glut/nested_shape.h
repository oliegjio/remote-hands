#pragma once

#include "shape.h"

struct nested_shape {
    nested_shape() {};
    nested_shape(std::initializer_list<shape*> arguments);

    void draw() const;

    std::vector<shape*> shapes;

private:
    void draw_recursive(std::vector<shape*> shapes) const;
};

