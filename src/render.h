#pragma once

#include <memory>
#include <string>
#include <vector>

#include "shader.h"
#include "tgaimage.h"

namespace minirender {
class Scene;

enum class OutType : uint32_t { FILE, MEMORY };

class Renderer {
 public:
  friend class IShader;
  static const int width = 800;
  static const int height = 800;

  Renderer()
      : sharder_fact_(new ShaderFactory<GouraudShader>()),
        zbuffer_(width * height),
        image_(width, height, TGAImage::RGB),
        bit_img_(nullptr),
        type_(OutType::FILE) {}

  ~Renderer() = default;

  void SetScene(std::shared_ptr<Scene> s) { scene_ = s; }

  // void VertexProcess();
  // void TriangleProcess();
  // void Rasterization();
  // void FragmentProcess();

  void DrawTriangle(const std::vector<vec4>& points, std::shared_ptr<IShader> sharder);
  void RenderToBuffer(std::uint8_t* buffer);
  void RenderToFile(const std::string& filename);

 private:
  void Clear();
  std::shared_ptr<Scene> scene_;
  std::shared_ptr<ShaderFactory<GouraudShader>> sharder_fact_;

  // std::vector<vec3> processed_verts_;
  std::vector<double> zbuffer_;
  TGAImage image_;
  std::uint8_t* bit_img_;
  OutType type_;

  void SetColor(int x, int y, TGAColor c);
  void Render();
};

}  // namespace minirender