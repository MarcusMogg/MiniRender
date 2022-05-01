#include "render.h"

#include "scene.h"

using namespace minirender;

void Renderer::VertexProcess() {
  mat<4, 4> trans = mat<4, 4>::identity();

  const double w = width * 3 / 4;
  const double h = height * 3 / 4;
  const double margin = width / 8;
  // left corner of image is (0,0)
  // transfer from [-1,1]x[-1,1] to [margin, margin+width]x[margin, margin+height]
  trans = trans * ViewPointTransform({-1, 1}, {-1, 1}, {margin, margin + w}, {margin, margin + h});
  trans = trans * scene_->camera_.PerspectiveMatrix();
  trans = trans * scene_->camera_.TranformationMatrix();

  for (int i = 0; i < scene_->model_.nverts(); i++) {
    const vec4 v = trans * embed<4, 3>(scene_->model_.vert(i));
    processed_verts_.emplace_back(vec3(v[0] / v[3], v[1] / v[3], v[2]));
  }
}

void Renderer::TriangleProcess() {}

// not right
// p must in the plane of abc
static vec3 Barycentric(const std::vector<vec3>& tr, const vec3& p) {
  vec3 res = cross(
      vec3(tr[1].x - tr[0].x, tr[2].x - tr[0].x, tr[0].x - p.x),
      vec3(tr[1].y - tr[0].y, tr[2].y - tr[0].y, tr[0].y - p.y));
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

void Renderer::DrawTriangle(
    const std::vector<vec3>& points, const std::vector<vec3>& normals, const std::vector<vec2>& uv) {
  vec2 box_min = {points[0].x, points[0].y};
  vec2 box_max = {points[0].x, points[0].y};

  for (const auto& i : points) {
    box_max.x = std::min(std::max(i.x, box_max.x), width - 1.);
    box_max.y = std::min(std::max(i.y, box_max.y), height - 1.);
    box_min.x = std::max(std::min(i.x, box_min.x), 0.);
    box_min.y = std::max(std::min(i.y, box_min.y), 0.);
  }
  const vec3 pz = {points[0].z, points[1].z, points[2].z};
  vec3 theta;
  for (int j = 0; j < 3; j++) {
    theta[j] = normals[j].normalize() * scene_->light_.normalize();
  }
  auto& tex = scene_->model_.diffuse();

  for (double i = int(box_min.x); i <= box_max.x; i++) {
    for (double j = int(box_min.y); j <= box_max.y; j++) {
      const auto b = Barycentric(points, {i, j, 0});
      if (b.x < 0 || b.y < 0 || b.z < 0) {
        continue;
      }
      double z = pz * b;

      if (zbuffer_[int(i + j * width)] < z) {
        zbuffer_[int(i + j * width)] = z;
        vec2 pos;

        double the = theta * b;
        if (the < 0) continue;
        for (size_t t = 0; t < 3; t++) {
          const vec2& v0 = uv[t];

          const int x0 = v0.x * tex.width();
          const int y0 = v0.y * tex.height();
          pos.x += x0 * b[t];
          pos.y += y0 * b[t];
        }

        auto c = tex.get(int(pos.x), int(pos.y));
        image_.set(i, j, TGAColor(c[2] * the, c[1] * the, c[0] * the));
      }
    }
  }
}

void Renderer::Rasterization() {
  auto& model = scene_->model_;
  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<vec3> screen;
    std::vector<vec2> uv;
    std::vector<vec3> normal;
    for (int j = 0; j < 3; j++) {
      screen.emplace_back(processed_verts_[model.vertidx(i, j)]);
      uv.push_back(model.uv(i, j));
      normal.push_back(model.normal(i, j));
    }

    DrawTriangle(screen, normal, uv);
  }
}

void Renderer::RenderToBuffer(std::uint8_t* buffer) {
  Clear();
  VertexProcess();
  TriangleProcess();
  Rasterization();

  image_.WriteToBuffer(buffer);
}

void Renderer::RenderToFile(const std::string& filename) {
  Clear();

  VertexProcess();
  TriangleProcess();
  Rasterization();

  image_.write_tga_file(filename);
}
void Renderer::Clear() {
  image_.Clear();
  processed_verts_.clear();
  std::fill(zbuffer_.begin(), zbuffer_.end(), -std::numeric_limits<double>::max());
}