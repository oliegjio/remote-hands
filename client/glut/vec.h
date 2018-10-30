#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/glut.h>
#include <vector>
#include <stdexcept>
#include <iostream>

template <size_t N>
class vec {
public:
	vec();
	vec(const vec &other);
	vec(const GLfloat &number);
	vec(std::initializer_list<GLfloat> arguments);

	size_t dimensions() const;
	void print() const;
	void normalize();
	float length() const;

	vec<N> &operator=(const vec<N> &other);

	vec<N> operator+(const vec<N> &other) const;
	vec<N> operator*(const vec<N> &other) const;

	vec<N> &operator+=(const vec<N> &other);
	vec<N> &operator*=(const vec<N> &other);
	vec<N> &operator/=(const vec<N> &other);

	vec<N> &operator/=(const GLfloat &number);

	bool operator==(const vec<N> &other) const;

	GLfloat operator[](const size_t &i) const;
	GLfloat &operator[](const size_t &i);

private:
    GLfloat data[N];
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;

template <size_t N>
vec<N>::vec() {
	for (size_t i = 0; i < N; i++) {
		data[i] = 0.0f;
	}
}

template <size_t N>
vec<N>::vec(const GLfloat &number) {
	for (size_t i = 0; i < N; i++) {
		data[i] = number;
	}
}

template <size_t N>
vec<N>::vec(std::initializer_list<GLfloat> arguments) {
	if (arguments.size() != N) {
		std::runtime_error("ERROR: Initializer list for `vec` class received wrong number of arguments!");
	}
	size_t i = 0;
	for (auto it = arguments.begin(); it != arguments.end(); ++it, i++) {
		data[i] = *it;
	}
}

template<size_t N>
vec<N> vec<N>::operator+(const vec<N> &other) const {
	vec<N> result;
	for (size_t i = 0; i < N; i++) {
		result = data[i] + other.data[i];
	}
	return result;
}

template<size_t N>
vec<N> vec<N>::operator*(const vec<N> &other) const {
	vec<N> result(1.0f);
	for (size_t i = 0; i < N; i++) {
		result = data[i] * other.data[i];
	}
	return result;
}

template<size_t N>
vec<N> &vec<N>::operator+=(const vec<N> &other) {
	for (size_t i = 0; i < N; i++) {
		data[i] += other.data[i];
	}
	return *this;
}

template<size_t N>
vec<N> &vec<N>::operator*=(const vec<N> &other) {
	for (size_t i = 0; i < N; i++) {
		data[i] *= other.data[i];
	}
	return *this;
}

template<size_t N>
vec<N> &vec<N>::operator/=(const vec<N> &other) {
	for (size_t i = 0; i < N; i++) {
		data[i] /= other.data[i];
	}
	return *this;
}

template<size_t N>
vec<N> & vec<N>::operator/=(const GLfloat &number) {
	for (size_t i = 0; i < N; i++) {
		data[i] /= number;
	}
	return *this;
}

template<size_t N>
bool vec<N>::operator==(const vec<N> &other) const {
	for (size_t i = 0; i < N; i++) {
		if (data[i] != other.data[i]) {
			return false;
		}
	}
	return true;
}

template<size_t N>
GLfloat vec<N>::operator[](const size_t &i) const {
	return data[i];
}

template<size_t N>
GLfloat &vec<N>::operator[](const size_t &i) {
	return data[i];
}

template <size_t N>
void vec<N>::print() const {
	std::cout << " (";
	for (size_t i = 0; i < N; i++) {
		if (i != N - 1) {
			std::cout << data[i] << ", ";
		} else {
			std::cout << data[i];
		}
	}
	std::cout << ") ";
}

template<size_t N>
size_t vec<N>::dimensions() const {
	return N;
}

template<size_t N>
vec<N> &vec<N>::operator=(const vec<N> &other) {
    if (this != &other) {
        for (size_t i = 0; i < N; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template<size_t N>
vec<N>::vec(const vec &other) {
    for (size_t i = 0; i < N; i++) {
        data[i] = other.data[i];
    }
}

template<size_t N>
void vec<N>::normalize() {
    float len = length();
    for (size_t i = 0; i < N; i++) {
        data[i] /= len;
    }
}

template<size_t N>
float vec<N>::length() const {
    float len = 0;
    for (size_t i = 0; i < N; i++) {
        len += std::pow(data[i], 2.0f);
    }

    return std::sqrt(len);
}
