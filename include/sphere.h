#ifndef SPHERE_H
#define SPHERE_H

#include "global.h"

#include "render_object.h"

class Sphere : public RenderObject {
  public:
    Sphere(Vec3 Center, double Radius, std::shared_ptr<Material> Mat): center(Center), radius(Radius), mat(Mat){}
    bool Hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;

private:
  Vec3 center;
  double radius;
  std::shared_ptr<Material> mat ;
};

#endif