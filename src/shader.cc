#include "shader.h"

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
}
vec4 GouraudShader::Vector(const int iface, const int nthvert) {
  varying_intensity_[nthvert] = std::max(0., model().normal(iface, nthvert).normalize() * light().normalize());
  vec4 res = transformation_ * embed<4, 3>(model().vert(iface, nthvert));
  return res / res[3];
}
bool GouraudShader::Fragement(const vec3 bar, TGAColor& c) {
  double the = varying_intensity_ * bar;
  c = TGAColor(255 * the, 255 * the, 255 * the);
  return false;
}
