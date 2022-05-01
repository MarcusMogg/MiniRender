#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tgaimage.h"

namespace minirender {
class Scene;

enum class OutType : uint32_t { FILE, MEMORY };

class Renderer {
 public:
  static const int width = 800;
  static const int height = 800;

  Renderer() : zbuffer_(width * height, -std::numeric_limits<double>::max()), image_(width, height, TGAImage::RGB) {}
  ~Renderer() = default;

  void SetScene(std::shared_ptr<Scene> s) { scene_ = s; }

  void VertexProcess();
  void TriangleProcess();
  void Rasterization();

  void DrawTriangle(const std::vector<vec3>& points, const std::vector<vec3>& normals, const std::vector<vec2>& uv);
  void RenderToBuffer(std::uint8_t* buffer);
  void RenderToFile(const std::string& filename);

 private:
  void Clear();
  std::shared_ptr<Scene> scene_;

  std::vector<vec3> processed_verts_;
  std::vector<double> zbuffer_;
  TGAImage image_;
  std::uint8_t* bit_img_;
  OutType type_;

  void SetColor(int x, int y, TGAColor c);
  void Render();
};

}  // namespace minirender