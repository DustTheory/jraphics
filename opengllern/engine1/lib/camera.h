#ifndef ENGINE1_LIB_CAMERA_H_
#define ENGINE1_LIB_CAMERA_H_

#include <eigen3/Eigen/Dense>

namespace Engine1 {

class Camera {
   public:
      void LookAt(const Eigen::Vector3f &look_at);
      void SetPosition(const Eigen::Vector3f &position);
      void SetUpVector(const Eigen::Vector3f &up);

      void SetNearZ(float near_z);
      void SetFarZ(float far_z);
      void SetAspectRatio(float aspect_ratio);

      void Rotate(float x, float y, float z);
      void Move(float x, float y, float z);
      void MoveRelativeToLook(float x, float y, float z);

      Eigen::Matrix4f GetViewTransformMatrix();
      Eigen::Matrix4f GetProjectionTransformMatrix();
      Eigen::Matrix4f GetProjectionViewTransformMatrix();

   private:
      Eigen::Vector3f position_ = {0.0f, 0.0f, 0.0f};
      Eigen::Vector3f look_at_ = {0.0f, 0.0f, 1.0f};
      Eigen::Vector3f up_ = {0.0f, 1.0f, 0.0f};
      Eigen::Vector3f right_ = {1.0f, 0.0f, 0.0f};

      float fov_ = 90.0f;
      float near_z_ = 1.0f;
      float far_z_ = 10.0f;
      float aspect_ratio_ = 1.0;

   };

} // namespace Engine1

#endif  // ENGINE1_LIB_CAMERA_H_