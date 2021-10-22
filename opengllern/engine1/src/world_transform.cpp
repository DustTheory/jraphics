#include "../lib/world_transform.h"
#include "../lib/mat_math.h"

using Engine1::WorldTransform;

void WorldTransform::SetScale(float x, float y, float z) {
    scale_ = {x, y, z};
}

void WorldTransform::SetRotation(float x, float y, float z) {
    rotation_ = {x, y, z};
}

void WorldTransform::SetPosition(float x, float y, float z) {
    position_ = {x, y, z};
}

void WorldTransform::Translate(float x, float y, float z) {
    position_.x() += x;
    position_.y() += y;
    position_.z() += z;
}

void WorldTransform::Rotate(float x, float y, float z) {
    rotation_.x() += x;
    rotation_.y() += y;
    rotation_.z() += z;
}

void WorldTransform::Scale(float x, float y, float z) {
    scale_.x() *= x;
    scale_.y() *= y;
    scale_.z() *= z;
}

Eigen::Matrix4f WorldTransform::GetMatrix() {
    Eigen::Matrix4f scale_mat = GenScaleMatrix(scale_.x(), scale_.y(), scale_.z());
    Eigen::Matrix4f rotate_mat = GenRotateMatrix(rotation_.x(), rotation_.y(), rotation_.z());
    Eigen::Matrix4f translate_mat = GenTranslateMatrix(position_.x(), position_.y(), position_.z());
    Eigen::Matrix4f world_transform_mat = translate_mat * rotate_mat * scale_mat;
    return world_transform_mat;
}
