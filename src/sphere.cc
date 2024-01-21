#include "../include/sphere.h"

bool Sphere::Hit(const Ray& ray, Interval rayScale, HitRecord& hitRecord) const {
  Vec3 originCenter = ray.Origin() - m_center;
  double a = ray.Direction().LengthSquared();
  // half of b
  double b = DotProduct(originCenter, ray.Direction());
  double c = originCenter.LengthSquared() - m_radius * m_radius;

  double discriminant = b * b - a * c;

  if (discriminant < 0)
    return false;

  double squareRootOfDiscriminant = std::sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range
  double root = (-b - squareRootOfDiscriminant) / a;
  if (!rayScale.Surrounds(root)) {
    root = (-b + squareRootOfDiscriminant) / a;
    if (!rayScale.Surrounds(root))
      return false;
  }

  hitRecord.m_scaler = root;
  hitRecord.m_point = ray.Scaler(hitRecord.m_scaler);
  Vec3 outwardNormal = (hitRecord.m_point - m_center) / m_radius;
  hitRecord.SetFaceNormal(ray, outwardNormal);
  hitRecord.m_material = m_mat;

  return true;
}