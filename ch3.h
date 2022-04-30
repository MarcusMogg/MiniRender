#pragma once

#include "model.h"
#include "triangle.h"

namespace minirender {

namespace ch3 {

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red = TGAColor(255, 0, 0, 255);
static const TGAColor green = TGAColor(0, 255, 0, 255);

void First(const std::string& file) {
  Model model(file);
  const double width = 800;
  const double height = 800;
  TGAImage image(width, height, TGAImage::RGB);
  std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
  const vec3 light_dir(0, 0, -1);  // define light_dir
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec3> screen;
    std::vector<vec3> word;
    for (int j = 0; j < 3; j++) {
      const vec3& v0 = model.vert(i, j);
      // 图像中间 对应模型中间
      // NOTICE: pixel coordinate shoulde be integer
      const double x0 = (v0.x + 1.) * width / 2.;
      const double y0 = (v0.y + 1.) * height / 2.;
      screen.emplace_back(x0, y0, v0.z);
      word.push_back(v0);
    }
    vec3 n = cross(word[2] - word[0], word[1] - word[0]).normalize();

    const double theta = n * light_dir;
    if (theta > 0) {
      Draw2dTriangleWithZbuffer(screen, TGAColor(255 * theta, 255 * theta, 255 * theta), image, zbuffer);
    }
  }

  image.write_tga_file("ch3_1.tga");
}

void Second(const std::string& file) {
  Model model(file);

  const double width = 800;
  const double height = 800;
  TGAImage image(width, height, TGAImage::RGB);
  std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
  const vec3 light_dir(0, 0, -1);  // define light_dir
  auto texure = model.diffuse();
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec3> screen;
    std::vector<vec3> word;
    std::vector<vec2> uv;
    for (int j = 0; j < 3; j++) {
      const vec3& v0 = model.vert(i, j);
      // 图像中间 对应模型中间
      const double x0 = (v0.x + 1.) * width / 2.;
      const double y0 = (v0.y + 1.) * height / 2.;
      screen.emplace_back(x0, y0, v0.z);
      word.push_back(v0);
      uv.push_back(model.uv(i, j));
    }
    vec3 n = cross(word[2] - word[0], word[1] - word[0]).normalize();

    const double theta = n * light_dir;

    if (theta > 0) {
      Draw2dTriangleWithZbuffer(screen, uv, texure, theta, image, zbuffer);
    }
  }

  image.write_tga_file("ch3_2.tga");
}

}  // namespace ch3

}  // namespace minirender