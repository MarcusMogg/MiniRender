#include "shader.h"

#include <cmath>

#include "camera.h"
#include "model.h"
#include "render.h"
#include "scene.h"
#include "transformations.h"

using namespace minirender;

const Model& IShader::model() { return renderer_->scene_->model_; }
const Camera& IShader::camera() { return renderer_->scene_->camera_; }
const vec3& IShader::light() { return renderer_->scene_->light_; }

void GouraudShader::Init(Renderer* r) {
  renderer_ = r;
  // for each frame GouraudShader transformation is same
  transformation_ = mat<4, 4>::identity();
  const double w = Renderer::width * 3 / 4;
  const double h = Renderer::height * 3 / 4;
  const double margin = Renderer::width / 8;

  transformation_ = transformation_ * ViewPointTransform({-1, 1}, {-1, 1}, {margin, margin + w}, {margin, margin + h});
  transformation_ = transformation_ * camera().PerspectiveMatrix();
  transformation_ = transformation_ * camera().TranformationMatrix();
  scala_uv_[0] = {1. * model().diffuse().width(), 0.};
  scala_uv_[1] = {0., 1. * model().diffuse().height()};

  uniform_m_ = camera().PerspectiveMatrix() * camera().TranformationMatrix();
  uniform_mit_ = uniform_m_.invert_transpose();
}
vec4 GouraudShader::Vector(const int iface, const int nthvert) {
  varying_uv_.set_col(nthvert, model().uv(iface, nthvert));
  // varying_intensity_[nthvert] = std::max(0., model().normal(iface, nthvert).normalize() * light().normalize());

  vec4 res = transformation_ * embed<4, 3>(model().vert(iface, nthvert));
  return res / res[3];
}

bool GouraudShader::Fragement(const vec3 bar, TGAColor& c) {
  vec2 tex = varying_uv_ * bar;
  vec3 n = proj<3>(uniform_mit_ * embed<4>(model().normal(tex))).normalize();
  vec3 l = proj<3>(uniform_m_ * embed<4>(light())).normalize();
  // after uniform, camera is aways (0,0,z)
  vec3 h = (l + vec3(0, 0, 1)).normalize();
  double diffuse = std::max(0., n * l);
  static const double ambient = 0.005;
  const int spec_w = model().specular().width();
  const int spec_h = model().specular().height();
  double spec = std::pow(std::max(0., n * h), model().specular().get(tex[0] * spec_w, tex[1] * spec_h)[0]);

  tex = scala_uv_ * tex;
  c = model().diffuse().get(tex.x, tex.y);
  for (int i = 0; i < 3; i++) {
    c[i] = std::min<float>(c[i] * (ambient + diffuse + 0.6 * spec), 255);
  }

  return false;
}
