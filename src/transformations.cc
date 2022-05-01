#include "transformations.h"
using namespace minirender;

mat<4, 4> minirender::CameraTransform(const vec3& camera, const vec3& look_center, const vec3& up) {
  const vec3 z = (camera - look_center).normalize();
  const vec3 x = cross(up, z).normalize();
  const vec3 y = cross(z, x).normalize();

  return {{
      {x.x, x.y, x.z, -look_center.x},
      {y.x, y.y, y.z, -look_center.y},
      {z.x, z.y, z.z, -look_center.z},
      {0, 0, 0, 1},
  }};
}

// assume after cameratransformat, camera pos is (0,0,z), look_center is (0,0,0)
mat<4, 4> minirender::PerspectiveTransform(const vec3& camera, const vec3& look_center) {
  return {{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, -1. / (camera - look_center).norm(), 1},
  }};
}

mat<4, 4> minirender::ViewPointTransform(const vec2& a, const vec2& b, const vec2& c, const vec2& d) {
  return {{
      {(c[1] - c[0]) / (a[1] - a[0]), 0, 0, (c[0] * a[1] - c[1] * a[0]) / (a[1] - a[0])},
      {0, (d[1] - d[0]) / (b[1] - b[0]), 0, (d[0] * b[1] - d[1] * b[0]) / (b[1] - b[0])},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }};
}
