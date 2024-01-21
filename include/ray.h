#pragma once
#include "vec3.h"

class Ray {
public:
  // contructors
  Ray() {}
  Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

  Vec3 Origin() const { return origin; }
  Vec3 Direction() const { return direction; }

  Vec3 Scaler(double value) const { return origin + value * direction; }

private:
  Vec3 origin;
  Vec3 direction;
};
