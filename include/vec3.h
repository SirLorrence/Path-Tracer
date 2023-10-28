#ifndef VEC3_H
#define VEC3_H

#include "global.h"
#include <iostream>

struct Vec3 {
  double axis[3];

  // initialtion constructors
  Vec3() : axis{0, 0, 0} {}; // default
  Vec3(double init_x, double init_y, double init_z)
      : axis{init_x, init_y, init_z} {};

  double x() const { return axis[0]; }
  double y() const { return axis[1]; }
  double z() const { return axis[2]; }

  // ======== vector operation functions
  // returns the inverse value of the vector
  Vec3 operator-() const { return Vec3(-axis[0], -axis[1], -axis[2]); };
  // returns the axis value based of an array element
  double operator[](int value) const { return axis[value]; }
  // returns the axis address value based on the element selected
  double &operator[](int value) { return axis[value]; }

  //
  Vec3 &operator+=(const Vec3 &input_vec3) {
    axis[0] += input_vec3[0];
    axis[1] += input_vec3[1];
    axis[2] += input_vec3[2];
    return *this; // dereference to the current vec3
  }

  Vec3 &operator*=(double value) {
    axis[0] *= value;
    axis[1] *= value;
    axis[2] *= value;
    return *this;
  }

  Vec3 &operator/=(double value) { return *this *= 1 / value; }

  // Vector Magnitude
  double LengthSquared() const {
    return axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
  }
  double Length() const { return std::sqrt(LengthSquared()); }

  static const Vec3 Zero() { return Vec3(0, 0, 0); }

  static Vec3 Random() {
    return Vec3(RandomDouble01(), RandomDouble01(), RandomDouble01());
  }
  static Vec3 Random(double min, double max) {
    return Vec3(RandomDouble(min, max), RandomDouble(min, max),
                RandomDouble(min, max));
  }

  bool NearZero() const {
    double sample = 1e-8;
    return (std::fabs(axis[0]) < sample && std::fabs(axis[1]) < sample && std::fabs(axis[2]) < sample);
  }
};

// Vector Utilies

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.axis[0] << ' ' << v.axis[1] << ' ' << v.axis[2];
}

// Math Operators
inline Vec3 operator+(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.axis[0] + b.axis[0], a.axis[1] + b.axis[1],
              a.axis[2] + b.axis[2]);
}
inline Vec3 operator-(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.axis[0] - b.axis[0], a.axis[1] - b.axis[1],
              a.axis[2] - b.axis[2]);
}

inline Vec3 operator*(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.axis[0] * b.axis[0], a.axis[1] * b.axis[1],
              a.axis[2] * b.axis[2]);
}
inline Vec3 operator*(double value, const Vec3 &v) {
  return Vec3(value * v.axis[0], value * v.axis[1], value * v.axis[2]);
}
inline Vec3 operator*(const Vec3 &v, double value) { return value * v; }
// using const for now
inline Vec3 operator/(const Vec3 &v, double value) { return (1 / value) * v; }

inline double DotProduct(const Vec3 &a, const Vec3 &b) {
  return a.axis[0] * b.axis[0] + a.axis[1] * b.axis[1] + a.axis[2] * b.axis[2];
}

inline Vec3 CrossProduct(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.y() * b.z() - a.z() * a.y(), a.z() * b.x() - a.x() * b.z(),
              a.x() * b.y() - a.y() * b.x());
}

// unit vector
inline Vec3 Normalized(Vec3 v) { return v / v.Length(); }

inline Vec3 RandomVec3InSphere() {
  while (true) {
    Vec3 point = Vec3::Random(-1, 1);
    if (point.LengthSquared() < 1)
      return point;
  }
}

inline Vec3 RandomNormalizedVec3() { return Normalized(RandomVec3InSphere()); }

inline Vec3 RandomInDisk(){
  while (true){
    Vec3 point = Vec3(RandomDouble(-1,1),RandomDouble(-1,1),0);
    if(point.LengthSquared() < 1) return point;
  }
}

inline Vec3 RandomOnHemisphere(const Vec3 &normal) {
  Vec3 unit_sphere_hit = RandomNormalizedVec3();
  if (DotProduct(unit_sphere_hit, normal) > 0.0)
    return unit_sphere_hit;
  return -unit_sphere_hit;
}

inline Vec3 Reflect(const Vec3 &v, const Vec3 &n){
  return v - 2 * DotProduct(v,n) *n;
}

inline Vec3 Refract(const Vec3 &v1, const Vec3 &v2, double etai_over_etat){
  auto cos_theta = std::fmin(DotProduct(-v1, v2), 1.0);
  Vec3 r_out_perp = etai_over_etat * (v1 + cos_theta * v2);
  Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.LengthSquared())) * v2;
  return r_out_perp + r_out_parallel;
}
#endif