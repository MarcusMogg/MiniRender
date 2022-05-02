#pragma once

#include <memory>

#include "geometry.h"
#include "tgaimage.h"

namespace minirender {

class Renderer;
class Scene;
class Camera;
class Model;

class IShader {
 protected:
  Renderer* renderer_;

  const Model& model();
  const Camera& camera();
  const vec3& light();

 public:
  IShader() : renderer_(nullptr) {}
  virtual ~IShader() {}

  virtual void Init(Renderer* r) { renderer_ = r; }
  virtual vec4 Vector(const int iface, const int nthvert) = 0;
  virtual bool Fragement(const vec3 bar, TGAColor& c) = 0;
};

template <typename T>
class ShaderFactory {
  static_assert(std::is_base_of_v<IShader, T>, "T must be a shader");

 public:
  std::shared_ptr<IShader> Create(Renderer* r) {
    std::shared_ptr<IShader> res = std::make_shared<T>();
    res->Init(r);
    return res;
  }
};

class GouraudShader : public IShader {
 public:
  GouraudShader() = default;
  virtual ~GouraudShader() override{};

  void Init(Renderer* r) override;
  vec4 Vector(const int iface, const int nthvert) override;
  bool Fragement(const vec3 bar, TGAColor& c) override;

 private:
  mat<4, 4> transformation_;
  vec3 varying_intensity_;  // written by vertex shader, read by fragment shader
};

}  // namespace minirender