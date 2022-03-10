#pragma once

namespace minirender {

struct Point2D {
  float x;
  float y;

  Point2D(float x, float y) : x(x), y(y) {}
};

Point2D operator-(const Point2D& l, const Point2D& r);
Point2D operator+(const Point2D& l, const Point2D& r);
Point2D operator*(const Point2D& l, const float r);

}  // namespace minirender