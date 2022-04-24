#ifndef ENGIN_E1_LIB_QUARTERNION_MATH_H_
#define ENGIN_E1_LIB_QUARTERNION_MATH_H

#include <eigen3/Eigen/Dense>

namespace engine1 {

Eigen::Quaternion<float> GenRotationQuarternion(float angle, Eigen::Vector3f axis);

} // namespace Engine1

#endif  // ENGIN_E1_LIB_QUARTERNION_MATH_H_
