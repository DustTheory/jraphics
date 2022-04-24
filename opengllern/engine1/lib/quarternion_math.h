#ifndef ENGINE1_LIB_QUARTERNION_MATH_H_
#define ENGINE1_LIB_QUARTERNION_MATH_H_

#include <eigen3/Eigen/Dense>

namespace Engine1 {

Eigen::Quaternion<float> GenRotationQuarternion(float angle, Eigen::Vector3f axis);

} // namespace Engine1

#endif  // ENGINE1_LIB_QUARTERNION_MATH_H_