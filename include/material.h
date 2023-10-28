#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"

#include "hittable-list.h"

struct HitRecord;

class Material {
public:
  virtual ~Material() = default;
  virtual bool Scatter(const Ray &input_ray, const HitRecord &hit_rec,
                       Color &attenuation, Ray &scattered_ray) const = 0;
};

class Lambertian : public Material { 
  public:
    Lambertian(const Color &color) : albedo(color) {}
    
    bool Scatter(const Ray &input_ray, const HitRecord &hit_rec, Color &attenuation, Ray &scattered_ray) const override {
      Vec3 scatter_direction = hit_rec.normal + RandomNormalizedVec3();
      
      if(scatter_direction.NearZero())
        scatter_direction = hit_rec.normal;
      
      scattered_ray = Ray(hit_rec.point, scatter_direction);
      attenuation = albedo;
      return true;
    }
    
    private:
    Color albedo;
};


class Metal : public Material {
  public: 
  Metal(const Color &color, double f) : albedo(color), fuzz(f < 1 ? f : 1) {}

   bool Scatter(const Ray &input_ray, const HitRecord &hit_rec, Color &attenuation, Ray &scattered_ray) const override {
    Vec3 reflected = Reflect(Normalized(input_ray.Direction()),hit_rec.normal);
    scattered_ray = Ray(hit_rec.point, reflected + fuzz * RandomNormalizedVec3());
    attenuation = albedo;
    return (DotProduct(scattered_ray.Direction(), hit_rec.normal));
   }
  

  private: 
  Color albedo;
  double fuzz;
};


class Dielectric : public Material{
  public: 
  Dielectric(double IndexOfRefraction ) : index_of_refraction(IndexOfRefraction) {}
   bool Scatter(const Ray &input_ray, const HitRecord &hit_rec, Color &attenuation, Ray &scattered_ray) const override{
    attenuation =  Color(1.0,1.0,1.0);
    double refraction_ratio = hit_rec.front_face ? (1.0/index_of_refraction) : index_of_refraction;

    Vec3 unit_direction = Normalized(input_ray.Direction());
    double cos_theta = std::fmin(DotProduct(-unit_direction, hit_rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
    
    bool cannot_refract = refraction_ratio * sin_theta > 1.0; 
    Vec3 dir;

    if(cannot_refract || Reflectance(cos_theta, refraction_ratio) > RandomDouble01())
      dir = Reflect(unit_direction,hit_rec.normal);
    else 
      dir = Refract(unit_direction,hit_rec.normal, refraction_ratio);


    scattered_ray = Ray(hit_rec.point, dir);
    return true;
   }
  
  private:
  double index_of_refraction;

  //Schlick's approximation 
  static double Reflectance (double cosine, double ref_index){
    double r0 = (1-ref_index) / (1+ref_index);
    r0 = r0*r0;
    return r0 + (1-r0)* std::pow((1-cosine),5);
  }
};

#endif