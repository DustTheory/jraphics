#include "../lib/quarternion_math.h"

Eigen::Quaternion<float> engine1::GenRotationQuarternion(float angle, Eigen::Vector3f axis){
    float cos_half = cos(angle/2);
    float sin_half = sin(angle/2);
    return Eigen::Quaternion<float>(cos_half, sin_half*axis.x(), sin_half*axis.y(), sin_half*axis.z());
}