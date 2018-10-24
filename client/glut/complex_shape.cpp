#include "complex_shape.h"

void complex_shape::add_shape(const shape &new_shape) {
    data.push_back(new_shape);
}

void complex_shape::draw() const {
    for (const auto &shape : data) {
        shape.draw();
    }
}