#pragma once

#include "geometry.h"
#include "tgaimage.h"

namespace minirender {

void DrawLineSimple(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image);
void DrawLineSimple2(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image);
void DrawLineSimple3(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image);

void DrawLine(const Point2d& begin, const Point2d& end, const TGAColor& color, TGAImage& image);
}  // namespace minirender