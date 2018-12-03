#pragma once

#include <vector>
#include <string>
#include <iostream>

template<typename T>
void print_vector(const std::vector<T> &v, const std::string &delimiter = " ") {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i];
        if (i != v.size() - 1) {
            std::cout << delimiter;
        }
    }
}
