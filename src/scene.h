#pragma once

#include "camera.h"
#include "model.h"

namespace minirender {

class Renderer;

class Scene {
  friend class Renderer;

 public:
  Scene() : light_(1, -1, 1) {}
  ~Scene() = default;

  bool AddModel(const std::string& filename) { return model_.LoadModel(filename); }
  int vert() { return model_.nverts(); }

 private:
  // assume there are only one model and center is (0,0,0)
  Model model_;
  Camera camera_;

  // TODO: ambient light and  point light
  vec3 light_;
};

}  // namespace minirender