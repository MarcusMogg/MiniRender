#pragma once

#include <vector>

#include "geometry.h"
#include "tgaimage.h"

namespace minirender {
void Draw2dTriangleUseLineSweep(const std::vector<vec<2>>& points, const TGAColor& color, TGAImage& image);
void Draw2dTriangle(const std::vector<vec<2>>& points, const TGAColor& color, TGAImage& image);
}  // namespace minirender