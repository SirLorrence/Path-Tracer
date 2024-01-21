#pragma once

#include "global.h"
#include "hittable-list.h"

struct HitRecord;

class Material {
public:
  virtual ~Material() = default;
  virtual bool Scatter(const Ray& input_ray, const HitRecord& hit_rec,
                       Color& attenuation, Ray& scattered_ray) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const Color& color) : albedo(color) {}

  bool Scatter(const Ray& inputRay, const HitRecord& hitRec, Color& attenuation, Ray& scatteredRay) const override {
    //thread_local  Vec3 scatter_direction = hit_rec.normal + RandomNormalizedVec3(); <--- no, bad lol
    Vec3 scatterDirection = hitRec.m_normal + RandomNormalizedVec3();

    if (scatterDirection.NearZero()) {
      scatterDirection = hitRec.m_normal;
    }
    scatteredRay = Ray(hitRec.m_point, scatterDirection);
    attenuation = albedo;
    return true;
  }

private:
  Color albedo;
};


class Metal : public Material {
public:
  Metal(const Color& color, double f) : albedo(color), fuzz(f < 1 ? f : 1) {}

  bool Scatter(const Ray& inputRay, const HitRecord& hitRec, Color& attenuation, Ray& scatteredRay) const override {
    Vec3 reflected = Reflect(Normalized(inputRay.Direction()), hitRec.m_normal);
    // thread_local Vec3 r = RandomNormalizedVec3();
    Vec3 dir = reflected + fuzz * RandomNormalizedVec3();
    scatteredRay = Ray(hitRec.m_point, dir);
    attenuation = albedo;
    return (DotProduct(scatteredRay.Direction(), hitRec.m_normal));
  }

private:
  Color albedo;
  double fuzz;
};


class Dielectric : public Material {
public:
  Dielectric(double IndexOfRefraction) : index_of_refraction(IndexOfRefraction) {}

  bool Scatter(const Ray& inputRay, const HitRecord& hitRec, Color& attenuation, Ray& scatteredRay) const override {
    attenuation = Color(1.0, 1.0, 1.0);
    double refractionRatio = hitRec.m_frontFace ? (1.0 / index_of_refraction) : index_of_refraction;

    const Vec3 unitDirection = Normalized(inputRay.Direction());
    const double cos_theta = std::fmin(DotProduct(-unitDirection, hitRec.m_normal), 1.0);
    const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    const bool cannotRefract = refractionRatio * sin_theta > 1.0;
    Vec3 dir;

    if (cannotRefract || Reflectance(cos_theta, refractionRatio) > RandomDouble01())
      dir = Reflect(unitDirection, hitRec.m_normal);
    else
      dir = Refract(unitDirection, hitRec.m_normal, refractionRatio);


    scatteredRay = Ray(hitRec.m_point, dir);
    return true;
  }

private:
  double index_of_refraction;

  //Schlick's approximation
  static double Reflectance(double cosine, double refIndex) {
    double r0 = (1 - refIndex) / (1 + refIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};
