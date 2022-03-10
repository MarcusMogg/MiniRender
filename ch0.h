#pragma once
#include "tgaimage.h"

namespace minirender {

namespace ch0 {

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red = TGAColor(255, 0, 0, 255);

void DrawRedPoint() {
  TGAImage image(100, 100, TGAImage::RGB);
  image.set(52, 41, red);
  image.flip_vertically();
  image.write_tga_file("output.tga");
}

}  // namespace ch0

}  // namespace minirender