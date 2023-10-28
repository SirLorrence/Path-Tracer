#include "../include/global.h"

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable-list.h"
#include "../include/material.h"
#include "../include/sphere.h"

int main() {
  // World Objects
  HittableList world;
  std::shared_ptr<Material> material_ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.Add(std::make_shared<Sphere> (Vec3(0, -1000, 0), 1000, material_ground));


  for (int i = -11; i < 11; i++) {
    for (int j = -11; j < 11; j++){
      double choose_material = RandomDouble01();
      Vec3 center (i + 0.9 * RandomDouble01(), 0.2, j + 0.9 * RandomDouble01());
      if((center - Vec3(4,0.2,0)).Length() > 0.9){
        std::shared_ptr<Material> sphere_material;

        if(choose_material < 0.8){
          Color albedo  = Color::Random() * Color::Random();
          sphere_material = std::make_shared<Lambertian>(albedo);
          world.Add(std::make_shared<Sphere>(center,0.2,sphere_material));
        }
        else if( choose_material < 0.95)  {
          Color albedo  = Color::Random(0.5,1);
          double fuzz = RandomDouble(0.5,1);
          sphere_material = std::make_shared<Metal>(albedo,fuzz);
          world.Add(std::make_shared<Sphere>(center,0.2,sphere_material));
        }
        else {
          sphere_material = std::make_shared<Dielectric>(1.5);
          world.Add(std::make_shared<Sphere>(center,0.2,sphere_material));
        }
      }
    }
  }

  std::shared_ptr<Material> material1 = std::make_shared<Dielectric>(1.5);
  world.Add(std::make_shared<Sphere>(Vec3(0,1,0),1.0, material1));
  std::shared_ptr<Material> material2 = std::make_shared<Lambertian>(Color(0.4,0.2,0.1));
  world.Add(std::make_shared<Sphere>(Vec3(-4,1,0),1.0, material2));
  std::shared_ptr<Material> material3 = std::make_shared<Metal>(Color(0.7,0.6,0.5),0.0);
  world.Add(std::make_shared<Sphere>(Vec3(4,1,0),1.0, material3));


  Camera main_camera;
  main_camera.aspect_ratio = 16.0 / 9.0;
  main_camera.img_width = 200;
  main_camera.pixel_sample_size = 100;
  main_camera.max_depth = 50;

  main_camera.v_fov = 20;
  main_camera.look_from = Vec3(13,2,3);
  main_camera.look_at = Vec3(0,0,0);
  
  main_camera.defocus_angle = 0.6;

  main_camera.Render(world);
}
