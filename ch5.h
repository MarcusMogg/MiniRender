#pragma once

#include "model.h"
#include "triangle.h"

namespace minirender {

namespace ch5 {

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
  const vec3 camera(0, 0, 3);      // camera pos
  auto texure = model.diffuse();

  mat<4, 4> perspect;
  perspect[0] = {1, 0, 0, 0};
  perspect[1] = {0, 1, 0, 0};
  perspect[2] = {0, 0, 1, 0};
  perspect[3] = {0, 0, -1. / camera.z, 1};

  const double w = width * 3 / 4;
  const double h = height * 3 / 4;
  const double margin = width / 8;
  // left corner of image is (0,0)
  // transfer from [-1,1]x[-1,1] to [margin, margin+width]x[margin, margin+height]
  mat<4, 4> viewport;
  viewport[0] = {w / 2, 0, 0, margin + w / 2};
  viewport[1] = {0, h / 2, 0, margin + h / 2};
  viewport[2] = {0, 0, 1., 0};
  viewport[3] = {0, 0, 0, 1};

  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec3> screen;
    std::vector<vec2> uv;
    vec3 theta;
    for (int j = 0; j < 3; j++) {
      vec4 v0 = viewport * perspect * embed<4, 3>(model.vert(i, j));
      vec3 v1 = {1. * v0[0] / v0[3], v0[1] / v0[3], v0[2]};
      screen.emplace_back(v1);
      uv.push_back(model.uv(i, j));
      // light_dir should be vertex to the light source
      // it means the negative light dir
      theta[j] = -1 * model.normal(i, j).normalize() * light_dir;
    }

    Draw2dTriangleWithZbuffer(screen, uv, texure, theta, image, zbuffer);
  }

  image.write_tga_file("ch5_1.tga");
}

}  // namespace ch5

}  // namespace minirender