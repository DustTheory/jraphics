

#ifndef MAT_MATH_H
#define MAT_MATH_H

#include <eigen3/Eigen/Dense>


Eigen::Matrix4f genRotateMatrix(float radians, Eigen::Vector3f axis);
Eigen::Matrix4f genTranslateMatrix(float x, float y, float z);
Eigen::Matrix4f genScaleMatrix(float x, float y, float z);

Eigen::Matrix4f genViewMatrix(Eigen::Vector3f location, Eigen::Vector3f lookAt, Eigen::Vector3f up);
Eigen::Matrix4f genProjectionMatrix(float fov, float aspect_ratio, float nearZ, float farZ);
#endif