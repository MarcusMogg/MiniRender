#pragma once
#include "line.h"
#include "tgaimage.h"
namespace minirender {

namespace ch1 {

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red = TGAColor(255, 0, 0, 255);

void FirstAttempt() {
  TGAImage image(100, 100, TGAImage::RGB);
  DrawLineSimple({0, 0}, {50, 50}, red, image);
  image.write_tga_file("output.tga");
}

}  // namespace ch1

}  // namespace minirender