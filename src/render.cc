#include "render.h"

#include "scene.h"

using namespace minirender;

// not right
// p must in the plane of abc
static vec3 Barycentric(const std::vector<vec4>& tr, const vec3& p) {
  vec3 res = cross(
      vec3(tr[1][0] - tr[0][0], tr[2][0] - tr[0][0], tr[0][0] - p.x),
      vec3(tr[1][1] - tr[0][1], tr[2][1] - tr[0][1], tr[0][1] - p.y));
  if (fabs(res.z) < eps) {
    // cross result zero
    // impossiable if param right
    return {-1, 1, -1};
  } else {
    res.x /= res.z;
    res.y /= res.z;
    // notice order
    // finalres[0] * tr[0] +  finalres[1] * tr[1] + finalres[2] * tr[2] = p
    return {1 - res.x - res.y, res.x, res.y};
  }
}

void Renderer::DrawTriangle(const std::vector<vec4>& points, std::shared_ptr<IShader> sharder) {
  vec2 box_min = {points[0][0], points[0][1]};
  vec2 box_max = {points[0][0], points[0][1]};

  for (const auto& i : points) {
    box_max.x = std::min(std::max(i[0], box_max.x), width - 1.);
    box_max.y = std::min(std::max(i[1], box_max.y), height - 1.);
    box_min.x = std::max(std::min(i[0], box_min.x), 0.);
    box_min.y = std::max(std::min(i[1], box_min.y), 0.);
  }
  const vec3 pz = {points[0][2], points[1][2], points[2][2]};

  for (double i = int(box_min.x); i <= box_max.x; i++) {
    for (double j = int(box_min.y); j <= box_max.y; j++) {
      const auto b = Barycentric(points, {i, j, 0});
      if (b.x < 0 || b.y < 0 || b.z < 0) {
        continue;
      }
      TGAColor c;
      if (sharder->Fragement(b, c)) {
        continue;
      }

      double z = pz * b;

      if (zbuffer_[int(i + j * width)] < z) {
        zbuffer_[int(i + j * width)] = z;
        SetColor(i, j, c);
      }
    }
  }
}

void Renderer::RenderToBuffer(std::uint8_t* buffer) {
  type_ = OutType::MEMORY;
  bit_img_ = buffer;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      bit_img_[(i + j * width) * 4 + 3] = 0xff;
    }
  }
  Render();
}

void Renderer::RenderToFile(const std::string& filename) {
  image_.Clear();
  type_ = OutType::FILE;
  Render();
  image_.write_tga_file(filename);
}

void Renderer::Render() {
  Clear();

  auto& model = scene_->model_;
  for (int i = 0; i < model.nfaces(); i++) {
    auto sharder = sharder_fact_->Create(this);
    std::vector<vec4> screen;
    for (int j = 0; j < 3; j++) {
      screen.emplace_back(sharder->Vector(i, j));
    }

    DrawTriangle(screen, sharder);
  }
}

void Renderer::Clear() {
  // processed_verts_.clear();
  std::fill(zbuffer_.begin(), zbuffer_.end(), -std::numeric_limits<double>::max());
}

void Renderer::SetColor(int x, int y, TGAColor c) {
  if (type_ == OutType::FILE) {
    image_.set(x, y, c);
  } else {
    y = height - y - 1;
    int idx = (x + y * width) * 4;
    bit_img_[idx] = c[2];
    bit_img_[idx + 1] = c[1];
    bit_img_[idx + 2] = c[0];
    bit_img_[idx + 3] = c[3];
  }
}