#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/glut.h>
#include <stddef.h>
#include <iostream>
#include <iomanip>

#include "vec.h"

template<size_t R, size_t C>
class matrix {
public:
    matrix(const GLfloat *data);

    void print() const;

    vec<R> operator*(const vec<C> &other) const;

private:
    size_t rows = R;
    size_t columns = C;

    GLfloat data[R * C];
};

typedef matrix<3, 3> matrix3;
typedef matrix<4, 4> matrix4;

template<size_t R, size_t C>
matrix<R, C>::matrix(const GLfloat *data) {
    for (size_t i = 0; i < R * C; i++) {
        this->data[i] = data[i];
    }
}

template<size_t R, size_t C>
vec<R> matrix<R, C>::operator*(const vec<C> &other) const {
    vec<R> result;
    for (size_t i = 0; i < R; i++) {
        for (size_t j = 0; j < C; j++) {
            result[i] += data[j + i * columns] * other[j];
        }
    }
    return result;
}

template<size_t R, size_t C>
void matrix<R, C>::print() const {
    for (size_t i = 0; i < R; i++) {
        for (size_t j = 0; j < C; j++) {
            std::cout << std::fixed  << std::setprecision(4) << data[j + i * C] << "   ";
        }
        std::cout << std::endl;
    }
}

