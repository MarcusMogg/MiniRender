#pragma once

#include "model.h"
#include "transformations.h"
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

  mat<4, 4> perspect = PerspectiveTransform(camera, {0, 0, 0});

  const double w = width * 3 / 4;
  const double h = height * 3 / 4;
  const double margin = width / 8;
  // left corner of image is (0,0)
  // transfer from [-1,1]x[-1,1] to [margin, margin+width]x[margin, margin+height]
  mat<4, 4> viewport = ViewPointTransform({-1, 1}, {-1, 1}, {margin, margin + w}, {margin, margin + h});

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

void Second(const std::string& file) {
  Model model(file);

  const double width = 800;
  const double height = 800;
  TGAImage image(width, height, TGAImage::RGB);
  std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
  const vec3 light_dir = vec3(1, -1, 1).normalize();  // define light_dir
  const vec3 camera(1, 1, 3);                         // camera pos
  const vec3 center(0, 0, 0);                         //
  auto texure = model.diffuse();

  mat<4, 4> cameratrans = CameraTransform(camera, center, {0, 1, 0});
  mat<4, 4> perspect = PerspectiveTransform(camera, center);

  const double w = width * 3 / 4;
  const double h = height * 3 / 4;
  const double margin = width / 8;
  // left corner of image is (0,0)
  // transfer from [-1,1]x[-1,1] to [margin, margin+width]x[margin, margin+height]
  mat<4, 4> viewport = ViewPointTransform({-1, 1}, {-1, 1}, {margin, margin + w}, {margin, margin + h});

  const mat<4, 4> alltrans = viewport * perspect * cameratrans;
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec3> screen;
    std::vector<vec2> uv;
    vec3 theta;
    for (int j = 0; j < 3; j++) {
      vec4 v0 = alltrans * embed<4, 3>(model.vert(i, j));
      vec3 v1 = {v0[0] / v0[3], v0[1] / v0[3], v0[2]};
      screen.emplace_back(v1);
      uv.push_back(model.uv(i, j));
      // light_dir should be vertex to the light source
      // it means the negative light dir
      theta[j] = model.normal(i, j).normalize() * light_dir;
    }

    Draw2dTriangleWithZbuffer(screen, uv, texure, theta, image, zbuffer);
  }

  image.write_tga_file("ch5_2.tga");
}

}  // namespace ch5

}  // namespace minirender