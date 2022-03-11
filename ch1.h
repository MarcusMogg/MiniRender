#pragma once
#include "line.h"
#include "model.h"
#include "tgaimage.h"
namespace minirender {

namespace ch1 {

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red = TGAColor(255, 0, 0, 255);

void FirstAttempt() {
  TGAImage image(100, 100, TGAImage::RGB);
  DrawLineSimple({0, 0}, {100, 20}, red, image);

  DrawLineSimple2({0, 5}, {100, 25}, red, image);
  DrawLineSimple2({100, 25}, {0, 5}, white, image);  // not work

  DrawLineSimple3({0, 10}, {100, 30}, red, image);
  DrawLineSimple3({100, 30}, {0, 10}, white, image);

  DrawLine({0, 20}, {100, 40}, red, image);
  DrawLine({100, 40}, {0, 20}, white, image);
  DrawLine({0, 25}, {100, 70}, red, image);
  image.write_tga_file("output.tga");
}

void DrawWireframe(const std::string& file) {
  Model model(file);
  const double width = 800;
  const double height = 800;
  TGAImage image(width, height, TGAImage::RGB);
  for (int i = 0; i < model.nfaces(); i++) {
    for (int j = 0; j < 3; j++) {
      vec3 v0 = model.vert(i, j);
      vec3 v1 = model.vert(i, (j + 1) % 3);
      double x0 = (v0.x + 1.) * width / 2.;
      double y0 = (v0.y + 1.) * height / 2.;
      double x1 = (v1.x + 1.) * width / 2.;
      double y1 = (v1.y + 1.) * height / 2.;
      DrawLine({x0, y0}, {x1, y1}, white, image);
    }
  }
  image.write_tga_file("Wireframe.tga");
}

}  // namespace ch1

}  // namespace minirender