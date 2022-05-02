#pragma once

#include "geometry.h"
#include "transformations.h"

namespace minirender {
class Camera {
 public:
  Camera() : pos_(1, 1, 3), up_(0, 1, 0), look_at_(0, 0, 0) {}
  ~Camera() = default;

  void Orbit() {}
  void Dolly() {}
  void Pan() {}

  mat<4, 4> TranformationMatrix() const { return CameraTransform(pos_, look_at_, up_); }
  mat<4, 4> PerspectiveMatrix() const { return PerspectiveTransform(pos_, look_at_); }

 private:
  vec3 pos_;
  vec3 up_;
  vec3 look_at_;
};

}  // namespace minirender