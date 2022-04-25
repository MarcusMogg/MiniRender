#include "triangle.h"

#include <algorithm>

#include "line.h"
using namespace minirender;

static void FillBetweenline(
    const vec2& same_point, const vec2& p1, const vec2& p2, const TGAColor& color, TGAImage& image) {
  const double k1 = (p1.y - same_point.y);
  const double k2 = (p2.y - same_point.y);
  double l, r;
  if (std::abs(p1.y - same_point.y) < std::abs(p2.y - same_point.y)) {
    l = std::min(p1.y, same_point.y);
    r = std::max(p1.y, same_point.y);
  } else {
    l = std::min(p2.y, same_point.y);
    r = std::max(p2.y, same_point.y);
  }
  const vec2 l1 = p1 - same_point;
  const vec2 l2 = p2 - same_point;
  for (; l <= r; l++) {
    const double x1 = (l - same_point.y) / k1;
    const double x2 = (l - same_point.y) / k2;
    DrawLine(same_point + l1 * x1, same_point + l2 * x2, color, image);
  }
}

void minirender::Draw2dTriangleUseLineSweep(const std::vector<vec2>& points, const TGAColor& color, TGAImage& image) {
  std::vector<vec2> points_copy = points;
  std::sort(points_copy.begin(), points_copy.end(), [](const auto& a, const auto& b) { return a.y < b.y; });

  FillBetweenline(points_copy[0], points_copy[1], points_copy[2], color, image);
  FillBetweenline(points_copy[2], points_copy[1], points_copy[0], color, image);
}

vec3 Barycentric(const std::vector<vec2>& tr, const vec2& p) {
  vec3 res = cross(
      vec3(tr[1].x - tr[0].x, tr[2].x - tr[0].x, tr[0].x - p.x),
      vec3(tr[1].y - tr[0].y, tr[2].y - tr[0].y, tr[0].y - p.y));
  if (fabs(res.z) < eps) {
    // cross result zero
    // impossiable if param right
    return {-1, 1, -1};
  } else {
    res.x /= res.z;
    res.y /= res.z;

    return {1 - res.x - res.y, res.x, res.y};
  }
}

void minirender::Draw2dTriangle(const std::vector<vec2>& points, const TGAColor& color, TGAImage& image) {
  vec2 box_min = points[0];
  vec2 box_max = points[0];
  for (const auto& i : points) {
    box_max.x = std::max(i.x, box_max.x);
    box_max.y = std::max(i.y, box_max.y);
    box_min.x = std::min(i.x, box_min.x);
    box_min.y = std::min(i.y, box_min.y);
  }
  for (double i = box_min.x; i < box_max.x; i++) {
    for (double j = box_min.y; j < box_max.y; j++) {
      const auto b = Barycentric(points, {i, j});
      if (b.x >= 0 && b.y >= 0 && b.z >= 0) {
        image.set(i, j, color);
      }
    }
  }
}
