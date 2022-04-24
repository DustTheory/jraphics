#ifndef ENGIN_E1_LIB_CAMERA_H_
#define ENGIN_E1_LIB_CAMERA_H_

#include <eigen3/Eigen/Dense>

namespace engine1 {

class Camera {
   public:
      void LookAt(const Eigen::Vector3f &look_at);
      void SetPosition(const Eigen::Vector3f &position);
      void SetUpVector(const Eigen::Vector3f &up_vector);

      void SetNearZ(float near_z);
      void SetFarZ(float far_z);
      void SetAspectRatio(float aspect_ratio);

      void Rotate(float x, float y, float z);
      void Move(float x, float y, float z);
      void MoveRelativeToLook(float x, float y, float z);

      Eigen::Matrix4f GetViewTransformMatrix();
      Eigen::Matrix4f GetProjectionTransformMatrix() const;
      Eigen::Matrix4f GetProjectionViewTransformMatrix();

   private:
      Eigen::Vector3f position_ = {0.0F, 0.0F, 0.0F};
      Eigen::Vector3f look_at_ = {0.0F, 0.0F, 1.0F};
      Eigen::Vector3f up_ = {0.0F, 1.0F, 0.0F};
      Eigen::Vector3f right_ = {1.0F, 0.0F, 0.0F};

      float fov_ = 90.0F;
      float near_z_ = 1.0F;
      float far_z_ = 10.0F;
      float aspect_ratio_ = 1.0;

   };

} // namespace Engine1

#endif  // ENGIN_E1_LIB_CAMERA_H_
