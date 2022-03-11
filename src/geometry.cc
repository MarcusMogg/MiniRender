#include "geometry.h"

namespace minirender {

// Point2d operator-(const Point2d& l, const Point2d& r) { return Point2d(l.x - r.x, l.y - r.y); };
// Point2d operator+(const Point2d& l, const Point2d& r) { return Point2d(l.x + r.x, l.y + r.y); };
// Point2d operator*(const Point2d& l, const float r) { return Point2d(l.x * r, l.y * r); };
vec3 cross(const vec3& v1, const vec3& v2) {
  return vec<3>{v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}
}  // namespace minirender