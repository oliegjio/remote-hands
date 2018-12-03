#include "quaternion.h"

quaternion::quaternion(const float &w, const float &x, const float &y, const float &z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

quaternion::quaternion(const vector3 &v) {
    this->w = 0;
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
}

matrix3 quaternion::to_matrix3() const {
    const GLfloat data[9] = {
            w * w + x * x - y * y - z * z, 2 * x * y - 2 * w * z, 2 * x * z + 2 * w * y,
            2 * x * y + 2 * w * z, w * w - x * x + y * y - z * z, 2 * y * z + 2 * w * x,
            2 * x * z - 2 * w * y, 2 * y * z - 2 * w * x, w * w - x * x - y * y + z * z
    };
    return matrix3(data);
}

matrix4 quaternion::to_matrix4() const {
    const GLfloat data[16] = {
            w * w + x * x - y * y - z * z, 2 * x * y - 2 * w * z, 2 * x * z + 2 * w * y, 0,
            2 * x * y + 2 * w * z, w * w - x * x + y * y - z * z, 2 * y * z + 2 * w * x, 0,
            2 * x * z - 2 * w * y, 2 * y * z - 2 * w * x, w * w - x * x - y * y + z * z, 0,
            0, 0, 0, 1
    };
    return matrix4(data);
}

quaternion quaternion::conjugate() const {
    return quaternion {w, -x, -y, -z};
}

vector3 quaternion::operator*(const vector3 &v) const {
    auto q = quaternion(v);
    auto result = (*this) * q * conjugate();
    return result.to_vector3();
}

quaternion quaternion::operator*(const quaternion &other) const {
    return quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w);
}

vector3 quaternion::to_vector3() const {
    return vector3 {x, y, z};
}
