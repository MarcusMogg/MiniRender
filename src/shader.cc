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

  varying_nrm_.set_col(nthvert, model().normal(iface, nthvert));
  vec4 res = transformation_ * embed<4, 3>(model().vert(iface, nthvert));
  res = res / res[3];
  varying_vert_[nthvert] = proj<3>(res);
  return res;
}

bool GouraudShader::Fragement(const vec3 bar, TGAColor& c) {
  vec2 tex = varying_uv_ * bar;
  vec3 bn = (varying_nrm_ * bar).normalize();  // per-vertex normal interpolation
  mat<3, 3> AI = mat<3, 3>{{varying_vert_[1] - varying_vert_[0], varying_vert_[2] - varying_vert_[0], bn}}.invert();
  vec3 i = AI * vec3(varying_uv_[0][1] - varying_uv_[0][0], varying_uv_[0][2] - varying_uv_[0][0], 0);
  vec3 j = AI * vec3(varying_uv_[1][1] - varying_uv_[1][0], varying_uv_[1][2] - varying_uv_[1][0], 0);

  mat<3, 3> B = mat<3, 3>{{i.normalize(), j.normalize(), bn}}.transpose();

  vec3 n = (B * (model().normal(tex))).normalize();  // transfer normal from texture to world space after uniform
  vec3 l = proj<3>(uniform_m_ * embed<4>(light())).normalize();
  // after uniform, camera is aways (0,0,z)
  vec3 h = (l + vec3(0, 0, 1)).normalize();
  double diffuse = std::max(0., n * l);
  static const double ambient = 10;
  const int spec_w = model().specular().width();
  const int spec_h = model().specular().height();
  double spec = std::pow(std::max(0., n * h), 5 + model().specular().get(tex[0] * spec_w, tex[1] * spec_h)[0]);

  tex = scala_uv_ * tex;
  c = model().diffuse().get(tex.x, tex.y);
  for (int i = 0; i < 3; i++) {
    c[i] = std::min<float>(ambient + c[i] * (diffuse + 0.6 * spec), 255);
  }

  return false;
}
