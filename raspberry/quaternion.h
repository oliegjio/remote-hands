#pragma "once"

#include "matrix.h"
#include "vector.h"

struct quaternion {
    quaternion(const float &w, const float &x, const float &y, const float &z);
    quaternion(const vector3 &v);

    matrix3 to_matrix3() const;
    matrix4 to_matrix4() const;

    vector3 operator*(const vector3 &v) const;
    quaternion operator*(const quaternion &other) const;

    vector3 to_vector3() const;

    quaternion conjugate() const;

    float w;
    float x;
    float y;
    float z;
};

