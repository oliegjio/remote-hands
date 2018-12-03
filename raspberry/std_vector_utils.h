#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <numeric>

std::vector<float> string_vector_to_float_vector(const std::vector<std::string> &v) {
    std::vector<float> result;

    for (size_t i = 0; i < v.size(); i++) {
        result.push_back(std::stof(v[i]));
    }

    return result;
}

std::string join_string_vector(const std::vector<std::string>& v)
{
    std::string result;
    for (auto const &s : v) {
        result += s;
    }
    return result;
}

template<typename T>
void print_vector(const std::vector<T> &v, const std::string &delimiter = " ") {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i];
        if (i != v.size() - 1) {
            std::cout << delimiter;
        }
    }
}
