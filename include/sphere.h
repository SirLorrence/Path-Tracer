#pragma once

#include "global.h"
#include "render_object.h"

class Sphere : public RenderObject {
public:
  Sphere(Vec3 center, double radius, std::shared_ptr<Material> mat): m_center(center), m_radius(radius), m_mat(mat) {}
  bool Hit(const Ray& r, Interval rayScale, HitRecord& rec) const override;

private:
  Vec3 m_center;
  double m_radius;
  std::shared_ptr<Material> m_mat;
};
