#ifndef _DISCRETE_FILTER_H_
#define _DISCRETE_FILTER_H_

#include <Arduino.h>

float clamp(float value, float min_value, float max_falue);

void filter(float ax, float ay, float az,
            float gx, float gy, float gz);

#endif //_DISCRETE_FILTER_H_
