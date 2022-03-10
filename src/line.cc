#include "line.h"

namespace minirender {

void DrawLineSimple(const Point2D& begin, const Point2D& end, const TGAColor& color, TGAImage& image) {
  for (float i = 0.; i < 1.; i += 0.01) {
    Point2D cur = (end - begin) * i + begin;
    image.set(cur.x, cur.y, color);
  }
}
}  // namespace minirender