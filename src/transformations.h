#pragma once

#include "geometry.h"

namespace minirender {

mat<4, 4> CameraTransform(const vec3& camera, const vec3& look_center, const vec3& up);

// assume after cameratransformat, camera pos is (0,0,z), look_center is (0,0,0)
mat<4, 4> PerspectiveTransform(const vec3& camera, const vec3& look_center);
mat<4, 4> ViewPointTransform(const vec2& a, const vec2& b, const vec2& c, const vec2& d);

}  // namespace minirender