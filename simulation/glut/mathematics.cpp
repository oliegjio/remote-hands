#include "mathematics.h"

#include <cmath>

float smooth_map(const float &x, const float &in_min, const float &in_max, const float &out_min, const float &out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float radians_to_degrees(const float &rad) {
    return rad * (180.0f / PI);
}

float degrees_to_radians(const float &deg) {
    return deg * (PI / 180.0f);
}

float clamp(const float &n, const float &minimum, const float &maximum) {
    return fmaxf(fminf(maximum, n), minimum);
}