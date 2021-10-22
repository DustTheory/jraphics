#include "../lib/camera.h"
#include "../lib/mat_math.h"

using Engine1::Camera;

void Camera::LookAt(Eigen::Vector3f look_at){
    look_at_ = look_at;
    look_at_.normalize();
    right_ = look_at_.cross(up_);
}
void Camera::SetPosition(Eigen::Vector3f position){
    position_ = position;
}
void Camera::SetUpVector(Eigen::Vector3f up){
    up_ = up;
    up_.normalize();
    right_ = look_at_.cross(up_);
}

void Camera::SetNearZ(float near_z){
    near_z_ = near_z;
}
void Camera::SetFarZ(float far_z){
    far_z_ = far_z;
}
void Camera::SetAspectRatio(float aspect_ratio){
    aspect_ratio_ = aspect_ratio;
}

void Camera::Rotate(float x, float y, float z){
    look_at_.x() += x;
    look_at_.y() += y;
    look_at_.z() += z;
    look_at_.normalize();

    up_.x() += x;
    up_.y() += y;
    up_.z() += z;
    up_.normalize();

    right_ = look_at_.cross(up_);
}
void Camera::Move(float x, float y, float z){
    position_.x() += x;
    position_.y() += y;
    position_.z() += z;
}

void Camera::MoveRelativeToLook(float x, float y, float z){
    position_ += look_at_*z;
    position_ += up_*y;
    position_ += right_ * x;
}

Eigen::Matrix4f Camera::GetViewTransformMatrix(){
    return GenViewMatrix(position_, look_at_, up_);
}
Eigen::Matrix4f Camera::GetProjectionTransformMatrix(){
    return GenProjectionMatrix(fov_, aspect_ratio_, near_z_, far_z_);
}
Eigen::Matrix4f Camera::GetProjectionViewTransformMatrix(){
    return GetProjectionTransformMatrix() * GetViewTransformMatrix();
}
