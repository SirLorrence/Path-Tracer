#include "../include/sphere.h"

bool Sphere::Hit(const Ray &ray, Interval ray_t, HitRecord &hit_record) const {
  Vec3 origin_center = ray.Origin() - center;
  double a = ray.Direction().LengthSquared();
  // half of b
  double b = DotProduct(origin_center, ray.Direction());
  double c = origin_center.LengthSquared() - radius * radius;

  double discriminant = b * b - a * c;

  if (discriminant < 0)
    return false;

  double square_root_of_discriminant = std::sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range
  double root = (-b - square_root_of_discriminant) / a;
  if (!ray_t.Surrounds(root)) {
    root = (-b + square_root_of_discriminant) / a;
    if (!ray_t.Surrounds(root))
      return false;
  }

  hit_record.scaler = root;
  hit_record.point = ray.Scaler(hit_record.scaler);
  Vec3 outward_normal = (hit_record.point - center) / radius;
  hit_record.SetFaceNormal(ray, outward_normal);
  hit_record.material = mat;

  return true;
}