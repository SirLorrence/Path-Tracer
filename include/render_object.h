#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "global.h"

class Material;

struct HitRecord {
  Vec3 point;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double scaler; // distance or scaler
  bool front_face;

  // Sets the hit record normal vector.

  void SetFaceNormal(const Ray &ray, const Vec3 &outward_normal) {
    front_face = DotProduct(ray.Direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class RenderObject {
public:
  virtual ~RenderObject() = default;
  virtual bool Hit(const Ray &r, Interval ray_t, HitRecord &rec) const = 0;
};

#endif