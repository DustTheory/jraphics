#include "../lib/mat_math.h"

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0f)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0f / M_PI)

Eigen::Matrix4f engine1::GenRotateAroundAxisMatrix(float radians, Eigen::Vector3f axis) {
    Eigen::Matrix4f c;
    axis.normalize();
    c << 0.0F,          -axis.z(),      axis.y(),       0.0F,
    axis.z(),      0.0F,           -axis.x(),      0.0F,
    -axis.y(),      axis.x(),       0.0F,           0.0F,
    0.0F,           0.0F,           0.0F,           0.0F;
    return Eigen::Matrix4f::Identity() + sin(radians)*c + (1-cos(radians))*(c*c);;
}

// Alternative implementation:

// https://www.songho.ca/opengl/files/gl_rotate27.png
// Eigen::Matrix4f engine1::genRotateMatrix(float radians, Eigen::Vector3f axis){
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


Eigen::Matrix4f engine1::GenTranslateMatrix(float x, float y, float z) {
    Eigen::Matrix4f c;
    c << 1.0F, 0.0F, 0.0F, x,
    0.0F, 1.0F, 0.0F, y,
    0.0F, 0.0F, 1.0F, z,
    0.0F, 0.0F, 0.0F, 1.0F;
    return c;
}

Eigen::Matrix4f engine1::GenScaleMatrix(float x, float y, float z) {
    Eigen::Matrix4f c;
    c << x, 0.0F, 0.0F, 0.0F,
        0.0F, y, 0.0F, 0.0F,
        0.0F, 0.0F, z, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F;
    return c;
}

Eigen::Matrix4f engine1::GenRotateMatrix(float x, float y, float z) {
    Eigen::Matrix4f rotate_x_mat;
    Eigen::Matrix4f rotate_y_mat;
    Eigen::Matrix4f rotate_z_mat;

    rotate_x_mat <<
                1.0F,   0.0F,   0.0F,    0.0F,
                0.0F,   cos(x), -sin(x), 0.0F,
                0.0F,   sin(x), cos(x),  0.0F,
                0.0F,   0.0F,   0.0F,    1.0F;

    rotate_y_mat <<
                cos(y),  0.0F,   sin(y),  0.0F,
                0.0F,    1.0F,   0.0F,    0.0F,
                -sin(y), 0.0F,   cos(y),  0.0F,
                0.0F,    0.0F,   0.0F,    1.0F;

    rotate_z_mat <<
                cos(z), -sin(z), 0.0F,  0.0F,
                sin(z), cos(z),  0.0F,  0.0F,
                0.0F,   0.0F,    1.0F,  0.0F,
                0.0F,   0.0F,    0.0F,  1.0F;
    return rotate_x_mat * rotate_y_mat * rotate_z_mat;
}

Eigen::Matrix4f engine1::GenViewMatrix(Eigen::Vector3f location, Eigen::Vector3f look_at, Eigen::Vector3f up) {
    look_at.normalize();
    up.normalize();
    auto right = look_at.cross(up);

    Eigen::Matrix4f rot_change_of_basis;
    rot_change_of_basis << right.x(), up.x(), look_at.x(), 0.0F,
                           right.y(), up.y(), look_at.y(), 0.0F,
                           right.z(), up.z(), look_at.z(), 0.0F,
                           0.0F,      0.0F,   0.0F,        1.0F;

    return rot_change_of_basis * (Eigen::Matrix4f() << 1.0F, 0.0F, 0.0F, -location.x(),
                                  0.0F, 1.0F, 0.0F, -location.y(),
                                  0.0F, 0.0F, 1.0F, -location.z(),
                                  0.0F, 0.0F, 0.0F, 1.0F).finished();
}

Eigen::Matrix4f engine1::GenProjectionMatrix(float fov, float aspect_ratio, float near_z, float far_z) {
    float tmp = tanf(DEGREES_TO_RADIANS(fov)/2.0F);
    return (Eigen::Matrix4f() << tmp/aspect_ratio, 0.0F, 0.0F,                         0.0F,
            0.0F,             tmp,  0.0F,                         0.0F,
            0.0F,             0.0F, (-far_z - near_z)/(near_z-far_z), 2*far_z*near_z/(near_z-far_z),
            0.0F,             0.0F, 1.0F,                         0.0F
           ).finished();
}
