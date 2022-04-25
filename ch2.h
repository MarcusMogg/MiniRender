#pragma once

#include "triangle.h"

namespace minirender {

namespace ch2 {

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red = TGAColor(255, 0, 0, 255);
static const TGAColor green = TGAColor(0, 255, 0, 255);

void First() {
  TGAImage image(200, 200, TGAImage::RGB);
  Draw2dTriangleUseLineSweep({vec2(10, 70), vec2(50, 160), vec2(70, 80)}, red, image);
  Draw2dTriangleUseLineSweep({vec2(180, 50), vec2(150, 1), vec2(70, 180)}, white, image);
  Draw2dTriangleUseLineSweep({vec2(180, 150), vec2(120, 160), vec2(130, 180)}, green, image);
  image.write_tga_file("ch2_1.tga");
}

void Second() {
  TGAImage image(200, 200, TGAImage::RGB);
  Draw2dTriangle({vec2(10, 70), vec2(50, 160), vec2(70, 80)}, red, image);
  Draw2dTriangle({vec2(180, 50), vec2(150, 1), vec2(70, 180)}, white, image);
  Draw2dTriangle({vec2(180, 150), vec2(120, 160), vec2(130, 180)}, green, image);
  image.write_tga_file("ch2_2.tga");
}

void Third(const std::string& file) {
  Model model(file);
  const double width = 800;
  const double height = 800;
  TGAImage image(width, height, TGAImage::RGB);
  const vec3 light_dir(0, 0, -1);  // define light_dir
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec2> screen;
    std::vector<vec3> word;
    for (int j = 0; j < 3; j++) {
      const vec3& v0 = model.vert(i, j);
      // 图像中间 对应模型中间
      const double x0 = (v0.x + 1.) * width / 2.;
      const double y0 = (v0.y + 1.) * height / 2.;
      screen.emplace_back(x0, y0);
      word.push_back(v0);
    }
    vec3 n = cross(word[2] - word[0], word[1] - word[0]).normalize();

    double theta = n * light_dir;
    if (theta > 0) {
      Draw2dTriangle(screen, TGAColor(255 * theta, 255 * theta, 255 * theta), image);
    }
  }
  image.write_tga_file("ch2_3.tga");
}

}  // namespace ch2

}  // namespace minirender