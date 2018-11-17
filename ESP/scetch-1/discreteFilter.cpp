#include "discreteFilter.h"

#define KF 0.1

float data[6];

/*float clamp(float value, float min_value, float max_value) {
    value = ( value > max_value ) ? max_value : value;
    value = ( value < min_value ) ? min_value : value;
    return value;
}*/

void filter(float ax, float ay, float az,
            float gx, float gy, float gz) 
{
    data[0] = ax; data[1] = ay; data[2] = az;
    data[3] = gx; data[4] = gy; data[5] = gz;    
    
}
