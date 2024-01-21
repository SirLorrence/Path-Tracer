#pragma once

#include "global.h"
#include <iostream>

struct Vec3 {
  double m_axis[3];

  // initialtion constructors
  Vec3() : m_axis{0, 0, 0} {}; // default
  Vec3(double initX, double initY, double initZ) : m_axis{initX, initY, initZ} {};

  double x() const { return m_axis[0]; }
  double y() const { return m_axis[1]; }
  double z() const { return m_axis[2]; }

  // ======== vector operation functions
  // returns the inverse value of the vector
  Vec3 operator-() const { return Vec3(-m_axis[0], -m_axis[1], -m_axis[2]); };
  // returns the axis value based of an array element
  double operator[](int value) const { return m_axis[value]; }
  // returns the axis address value based on the element selected
  double& operator[](int value) { return m_axis[value]; }

  //
  Vec3& operator+=(const Vec3& inputVec3) {
    m_axis[0] += inputVec3[0];
    m_axis[1] += inputVec3[1];
    m_axis[2] += inputVec3[2];
    return *this; // dereference to the current vec3
  }

  Vec3& operator*=(const double value) {
    m_axis[0] *= value;
    m_axis[1] *= value;
    m_axis[2] *= value;
    return *this;
  }

  Vec3& operator/=(const double value) { return *this *= 1 / value; }

  // Vector Magnitude
  double LengthSquared() const {
    return m_axis[0] * m_axis[0] + m_axis[1] * m_axis[1] + m_axis[2] * m_axis[2];
  }

  double Length() const { return std::sqrt(LengthSquared()); }

  static Vec3 Zero() { return Vec3(0, 0, 0); }

  static Vec3 Random() { return Vec3(RandomDouble01(), RandomDouble01(), RandomDouble01()); }

  static Vec3 Random(double min, double max) {
    return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
  }

  bool NearZero() const {
    double sample = 1e-8;
    return (std::fabs(m_axis[0]) < sample && std::fabs(m_axis[1]) < sample && std::fabs(m_axis[2]) < sample);
  }
};

// Vector Utilies
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
  return out << v.m_axis[0] << ' ' << v.m_axis[1] << ' ' << v.m_axis[2];
}

// Math Operators
inline Vec3 operator+(const Vec3& a, const Vec3& b) {
  return Vec3(a.m_axis[0] + b.m_axis[0], a.m_axis[1] + b.m_axis[1], a.m_axis[2] + b.m_axis[2]);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
  return Vec3(a.m_axis[0] - b.m_axis[0], a.m_axis[1] - b.m_axis[1], a.m_axis[2] - b.m_axis[2]);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b) {
  return Vec3(a.m_axis[0] * b.m_axis[0], a.m_axis[1] * b.m_axis[1], a.m_axis[2] * b.m_axis[2]);
}

inline Vec3 operator*(double value, const Vec3& v) {
  return Vec3(value * v.m_axis[0], value * v.m_axis[1], value * v.m_axis[2]);
}

inline Vec3 operator*(const Vec3& v, double value) { return value * v; }
// using const for now
inline Vec3 operator/(const Vec3& v, double value) { return (1 / value) * v; }

inline double DotProduct(const Vec3& a, const Vec3& b) {
  return a.m_axis[0] * b.m_axis[0] + a.m_axis[1] * b.m_axis[1] + a.m_axis[2] * b.m_axis[2];
}

inline Vec3 CrossProduct(const Vec3& a, const Vec3& b) {
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

inline Vec3 RandomInDisk() {
  while (true) {
    Vec3 point = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
    if (point.LengthSquared() < 1) return point;
  }
}

inline Vec3 RandomOnHemisphere(const Vec3& normal) {
  Vec3 unitSphereHit = RandomNormalizedVec3();
  if (DotProduct(unitSphereHit, normal) > 0.0)
    return unitSphereHit;
  return -unitSphereHit;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * DotProduct(v, n) * n;
}

inline Vec3 Refract(const Vec3& v1, const Vec3& v2, double etaiOverEtat) {
  double cosTheta = std::fmin(DotProduct(-v1, v2), 1.0);
  Vec3 rOutPerp = etaiOverEtat * (v1 + cosTheta * v2);
  Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * v2;
  return rOutPerp + rOutParallel;
}
