#pragma once
#include "global.h"

class Material;

struct HitRecord {
  // Sets the hit record normal vector.
  void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
    m_frontFace = DotProduct(ray.Direction(), outwardNormal) < 0;
    m_normal = m_frontFace ? outwardNormal : -outwardNormal;
  }

  Vec3 m_point;
  Vec3 m_normal;
  std::shared_ptr<Material> m_material;
  double m_scaler; // distance or scaler
  bool m_frontFace;
};

class RenderObject {
public:
  virtual ~RenderObject() = default;
  virtual bool Hit(const Ray& r, Interval rayScale, HitRecord& rec) const = 0;
};
