#include "../lib/camera.h"
#include "../lib/mat_math.h"
#include "../lib/quarternion_math.h"

using engine1::Camera;

void Camera::LookAt(const Eigen::Vector3f &look_at){
    look_at_ = look_at;
    look_at_.normalize();
    right_ = up_.cross(look_at_);
}
void Camera::SetPosition(const Eigen::Vector3f &position){
    position_ = position;
}
void Camera::SetUpVector(const Eigen::Vector3f &up){
    up_ = up;
    up_.normalize();
    right_ = up_.cross(look_at_);
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

    auto rotate_vertical_quarternion = engine1::GenRotationQuarternion(z, right_);
    look_at_ = rotate_vertical_quarternion*look_at_;
    up_ = rotate_vertical_quarternion*up_;
    right_ = up_.cross(look_at_);
    
    auto rotate_horizintal_quarternion = engine1::GenRotationQuarternion(-y, {0.0F, 1.0F, 0.0F});
    look_at_ = rotate_horizintal_quarternion*look_at_;
    up_ = rotate_horizintal_quarternion*up_;
    right_ = up_.cross(look_at_);

}
void Camera::Move(float x, float y, float z){
    position_.x() += x;
    position_.y() += y;
    position_.z() += z;
}

void Camera::MoveRelativeToLook(float x, float y, float z){

    position_ += Eigen::Vector3f(look_at_.x(), -look_at_.y(), look_at_.z()).normalized()*z;
    position_ += Eigen::Vector3f(0.0F, 1.0F, 0.0F)*y;
    position_ += Eigen::Vector3f(right_.x(), 0.0F, right_.z()).normalized() * x;
}

Eigen::Matrix4f Camera::GetViewTransformMatrix(){
    return GenViewMatrix(position_, look_at_, up_);
}
Eigen::Matrix4f Camera::GetProjectionTransformMatrix() const{
    return GenProjectionMatrix(fov_, aspect_ratio_, near_z_, far_z_);
}
Eigen::Matrix4f Camera::GetProjectionViewTransformMatrix(){
    return GetProjectionTransformMatrix() * GetViewTransformMatrix();
}
