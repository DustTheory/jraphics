#include "../lib/matMath.h"
#include<iostream>

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0f)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0f / M_PI)

Eigen::Matrix4f genRotateMatrix(float radians, Eigen::Vector3f axis){
    Eigen::Matrix4f C;
    axis.normalize();
    C << 0.0f,          -axis.z(),      axis.y(),       0.0f,
         axis.z(),      0.0f,           -axis.x(),      0.0f,
        -axis.y(),      axis.x(),       0.0f,           0.0f,
        0.0f,           0.0f,           0.0f,           0.0f;
    return Eigen::Matrix4f::Identity() + sin(radians)*C + (1-cos(radians))*(C*C);;
}

// https://www.songho.ca/opengl/files/gl_rotate27.png
// Eigen::Matrix4f genRotateMatrix(float radians, Eigen::Vector3f axis){
//     float c = cosf(radians);
//     float s = sinf(radians);
//     axis.normalize();
//     float x = axis.x();
//     float y = axis.y();
//     float z = axis.z();
//     Eigen::Matrix4f C;
//     C << (1.0f-c)*x*x + c,  (1.0f-c)*x*y - s*z, (1.0f-c)*x*z + s*y, 0.0f,
//          (1.0-c)*x*y + s*z, (1.0f-c)*y*y + c,   (1.0-c)*y*z - s*x,  0.0f,
//          (1.0-c)*x*z - s*y, (1.0f-c)*y*z + s*x, (1.0-c)*z*z + c,    0.0f,
//           0.0f,              0.0f,               0.0f,              1.0f;
//     return C;
// }


Eigen::Matrix4f genTranslateMatrix(float x, float y, float z){
    Eigen::Matrix4f C;
    C << 1.0f, 0.0f, 0.0f, x   ,
         0.0f, 1.0f, 0.0f, y   ,
         0.0f, 0.0f, 1.0f, z   ,
         0.0f, 0.0f, 0.0f, 1.0f;
    return C;
}

Eigen::Matrix4f genScaleMatrix(float x, float y, float z){
    Eigen::Matrix4f C;
    C << x   , 0.0f, 0.0f, 0.0f,
         0.0f, y   , 0.0f, 0.0f,
         0.0f, 0.0f, z   , 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f;
    return C;
}

Eigen::Matrix4f genViewMatrix(Eigen::Vector3f location, Eigen::Vector3f lookAt, Eigen::Vector3f up) {
    lookAt.normalize();
    up.normalize();
    auto right = lookAt.cross(up);

    Eigen::Matrix4f rotChangeOfBasis;
    rotChangeOfBasis << right.x(), up.x(), lookAt.x(), 0.0f,
                        right.y(), up.y(), lookAt.y(), 0.0f,
                        right.z(), up.z(), lookAt.z(), 0.0f,
                        0.0f,      0.0f,   0.0f,       1.0f;

    return rotChangeOfBasis * (Eigen::Matrix4f() << 1.0f, 0.0f, 0.0f, -location.x(),
                                 0.0f, 1.0f, 0.0f, -location.y(),
                                 0.0f, 0.0f, 1.0f, -location.z(),
                                 0.0f, 0.0f, 0.0f, 1.0f).finished();
}

Eigen::Matrix4f genProjectionMatrix(float fov, float aspect_ratio, float nearZ, float farZ){
    float tmp = tanf(degreesToRadians(fov)/2.0f);
    return (Eigen::Matrix4f() << tmp/aspect_ratio, 0.0f, 0.0f,                         0.0f,
                                 0.0f,             tmp,  0.0f,                         0.0f,
                                 0.0f,             0.0f, (-farZ - nearZ)/(nearZ-farZ), 2*farZ*nearZ/(nearZ-farZ),
                                 0.0f,             0.0f, 1.0f,                         0.0f
            ).finished();
}

