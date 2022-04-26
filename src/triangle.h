#pragma once

#include <vector>

#include "geometry.h"
#include "tgaimage.h"

namespace minirender {
void Draw2dTriangleUseLineSweep(const std::vector<vec2>& points, const TGAColor& color, TGAImage& image);
void Draw2dTriangle(const std::vector<vec2>& points, const TGAColor& color, TGAImage& image);
void Draw2dTriangleWithZbuffer(
    const std::vector<vec3>& points, const TGAColor& color, TGAImage& image, std::vector<double>& zbuffer);
void Draw2dTriangleWithZbuffer(
    const std::vector<vec3>& points, std::vector<TGAColor>& texture, TGAImage& image, std::vector<double>& zbuffer);
}  // namespace minirender