#include "geometry.h"

namespace minirender {

Point2D operator-(const Point2D& l, const Point2D& r) { return Point2D(l.x - r.x, l.y - r.y); };
Point2D operator+(const Point2D& l, const Point2D& r) { return Point2D(l.x + r.x, l.y + r.y); };
Point2D operator*(const Point2D& l, const float r) { return Point2D(l.x * r, l.y * r); };

}  // namespace minirender