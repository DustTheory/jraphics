#include "../lib/quarternion_math.h"

Eigen::Quaternion<float> Engine1::GenRotationQuarternion(float angle, Eigen::Vector3f axis){
    float cosHalf = cos(angle/2);
    float sinHalf = sin(angle/2);
    return Eigen::Quaternion<float>(cosHalf, sinHalf*axis.x(), sinHalf*axis.y(), sinHalf*axis.z());
}