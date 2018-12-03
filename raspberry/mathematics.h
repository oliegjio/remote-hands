#pragma once

#define PI 3.14159265f

float smooth_map(const float &x, const float &in_min, const float &in_max, const float &out_min, const float &out_max);

float radians_to_degrees(const float &rad);

float degrees_to_radians(const float &deg);

float clamp(const float &n, const float &minimum, const float &maximum);