

#ifndef ENGIN_E1_LIB_MAT_MATH_H_
#define ENGIN_E1_LIB_MAT_MATH_H

#include <eigen3/Eigen/Dense>

namespace engine1 {

Eigen::Matrix4f GenRotateAroundAxisMatrix(float radians, Eigen::Vector3f axis);
Eigen::Matrix4f GenRotateMatrix(float x, float y, float z);
Eigen::Matrix4f GenTranslateMatrix(float x, float y, float z);
Eigen::Matrix4f GenScaleMatrix(float x, float y, float z);

Eigen::Matrix4f GenViewMatrix(Eigen::Vector3f location, Eigen::Vector3f look_at, Eigen::Vector3f up);
Eigen::Matrix4f GenProjectionMatrix(float fov, float aspect_ratio, float near_z, float far_z);

} // namespace Engine1

#endif  // ENGIN_E1_LIB_MAT_MATH_H_
