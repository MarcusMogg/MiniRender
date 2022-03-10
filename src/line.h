#pragma once

#include "geometry.h"
#include "tgaimage.h"

namespace minirender {

void DrawLineSimple(const Point2D& begin, const Point2D& end, const TGAColor& color, TGAImage& image);

}  // namespace minirender