#include "line.h"

namespace minirender {

void DrawLineSimple(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image) {
  for (float i = 0.; i < 1.; i += 0.1) {
    Point2d cur = (end - begin) * i + begin;
    image.set(cur.x, cur.y, color);
  }
}

void DrawLineSimple2(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image) {
  // wrong
  // you can't assume begin.x < end.x
  for (int x = begin.x; x < end.x; ++x) {
    float t = (x - begin.x) / (end.x - begin.x);
    int y = begin.y + (end.y - begin.y) * t;
    image.set(x, y, color);
  }
}

void DrawLineSimple3(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image) {
  // x0 must less than x1
  const auto draw = [&image, &color](float x0, float y0, float x1, float y1, bool steep) {
    for (float x = x0; x <= x1; x++) {
      float t = (x - x0) / (x1 - x0);
      float y = y0 * (1. - t) + y1 * t;
      if (steep) {
        image.set(y, x, color);  // if transposed, de−transpose
      } else {
        image.set(x, y, color);
      }
    }
  };

  // draw more point
  if (std::abs(begin.x - end.x) > std::abs(begin.y - end.y)) {
    if (begin.x < end.x) {
      draw(begin.x, begin.y, end.x, end.y, false);
    } else {
      draw(end.x, end.y, begin.x, begin.y, false);
    }
  } else {
    if (begin.y < end.y) {
      draw(begin.y, begin.x, end.y, end.x, true);
    } else {
      draw(end.y, end.x, begin.y, begin.x, true);
    }
  }
}

void DrawLine(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image) {
  // x0 must less than x1
  const auto draw = [&image, &color](const float x0, const float y0, const float x1, const float y1, const bool steep) {
    const float dx = (x1 - x0);
    const float dy = (y1 - y0);
    const float derror = abs(dy / dx);
    const float dy_normal = y1 > y0 ? 1 : -1;
    float error = 0;
    float y = y0;
    for (float x = x0; x <= x1; x++) {
      if (steep) {
        image.set(y, x, color);  // if transposed, de−transpose
      } else {
        image.set(x, y, color);
      }

      error += derror;
      if (error > 0.5) {
        y += dy_normal;
        error -= 1;
      }
    }
  };

  // draw more point
  if (std::abs(begin.x - end.x) > std::abs(begin.y - end.y)) {
    if (begin.x < end.x) {
      draw(begin.x, begin.y, end.x, end.y, false);
    } else {
      draw(end.x, end.y, begin.x, begin.y, false);
    }
  } else {
    if (begin.y < end.y) {
      draw(begin.y, begin.x, end.y, end.x, true);
    } else {
      draw(end.y, end.x, begin.y, begin.x, true);
    }
  }
}

}  // namespace minirender